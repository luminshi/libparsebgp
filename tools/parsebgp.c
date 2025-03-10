/*
 * Copyright (C) 2017 The Regents of the University of California.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "parsebgp.h"
#include "config.h"
#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NAME "parsebgp"

// Read 1MB of the file at a time
#define BUFLEN (1024 * 1024)

static const char *type_strs[] = {
  NULL,  // PARSEBGP_MSG_TYPE_INVALID
  "bgp", // PARSEBGP_MSG_TYPE_BGP
  "bmp", // PARSEBGP_MSG_TYPE_BMP
  "mrt", // PARSEBGP_MSG_TYPE_MRT
  "openbmp", // PARSEBGP_MSG_TYPE_OPENBMP
};

// should messages NOT be dumped to stdout after parsing
//
// normally the debug output would be the only reason you would run this tool,
// but it may be useful to disable output to test raw parsing speed without all
// the printfs slowing things down.
static int silent = 0;

static ssize_t refill_buffer(FILE *fp, uint8_t *buf, size_t buflen,
                             size_t remain)
{
  size_t len = 0;

  if (remain > 0) {
    // need to move remaining data to start of buffer
    memmove(buf, buf + buflen - remain, remain);
    len += remain;
  }

  if (feof(fp) != 0) {
    // nothing more to read
    return len;
  }

  // do a read, we should get something at least
  len += fread(buf + len, 1, buflen - len, fp);

  if (ferror(fp) != 0) {
    return -1;
  }

  return len;
}

static int parse(parsebgp_opts_t *opts, parsebgp_msg_type_t type, char *fname)
{
  uint8_t buf[BUFLEN];
  FILE *fp = NULL;

  ssize_t fill_len = 0, remain = 0;
  size_t dec_len = 0;
  uint8_t *ptr;

  parsebgp_msg_t *msg = NULL;
  parsebgp_error_t err = PARSEBGP_OK;

  uint64_t cnt = 0;

  if ((msg = parsebgp_create_msg()) == NULL) {
    fprintf(stderr, "ERROR: Failed to create message structure\n");
    goto err;
  }

  if (strcmp(fname, "-") == 0) {
    fp = stdin;
  } else if ((fp = fopen(fname, "r")) == NULL) {
    fprintf(stderr, "ERROR: Could not open %s (%s)\n", fname, strerror(errno));
    goto err;
  }

  buf[0] = '\0';

  while ((fill_len = refill_buffer(fp, buf, BUFLEN, remain)) > 0) {
    if (fill_len == remain) {
      // failed to read anything new from the file, so give up
      fprintf(stderr,
              "ERROR: Possibly corrupt file encountered. Trailing garbage of "
              "%ld bytes found\n",
              remain);
      break;
    }
    remain = fill_len;
    ptr = buf;

    while (remain > 0) {
      dec_len = remain;
      if ((err = parsebgp_decode(*opts, type, msg, ptr, &dec_len)) !=
          PARSEBGP_OK) {
        if (err == PARSEBGP_PARTIAL_MSG) {
          // refill the buffer and try again
          parsebgp_clear_msg(msg);
          break;
        } else if (err == PARSEBGP_TRUNCATED_MSG && opts->ignore_invalid) {
          if (!(opts)->silence_invalid) {
            fprintf(stderr, "WARN: truncated message %" PRIu64 " in %s\n",
              cnt, fname);
          }
        } else {
          // else: its a fatal error
          fprintf(stderr, "ERROR: Failed to parse message (%d:%s)\n", err,
                  parsebgp_strerror(err));
          goto err;
        }
      }
      // else: successful read
      assert(dec_len > 0);
      ptr += dec_len;
      remain -= dec_len;
      cnt++;

      if (!silent) {
        parsebgp_dump_msg(msg);
      }

      parsebgp_clear_msg(msg);
    }
  }

  if (fill_len < 0) {
    fprintf(stderr, "ERROR: Failed to read from %s (%s)\n", fname,
            strerror(errno));
    goto err;
  }

  fprintf(stderr, "INFO: Read %" PRIu64 " messages from %s\n", cnt, fname);

  if (fp != NULL && fp != stdin) {
    fclose(fp);
  }

  parsebgp_destroy_msg(msg);

  return 0;

err:
  if (fp != NULL) {
    fclose(fp);
  }
  parsebgp_destroy_msg(msg);
  return -1;
}

static void usage(void)
{
  fprintf(
    stderr,
    "usage: %s [options] [type:]file [[type:]file...]\n"
    "         where 'type' is one of 'bmp', 'bgp', 'mrt', or 'openbmp'\n"
    "         (only required if using non-standard file extensions)\n"
    "       -4                 Force 4-byte ASN parsing\n"
    "       -b                 Perform shallow BMP parsing\n"
    "       -f <attr-type>     Filter to include given Path Attribute\n"
    "       -i                 Ignore invalid messages and attributes\n"
    "                            (use multiple times to silence warnings)\n"
    "       -s                 Skip unknown messages and attributes\n"
    "                            (use multiple times to silence warnings)\n"
    "       -m                 BGP messages do not include the 16-octet marker\n"
    "       -h                 Show this help message\n"
    "       -q                 Do not dump parsed messages (quiet mode)\n"
    "       -v                 Show version of the libparsebgp library\n",
    NAME);
}

int main(int argc, char **argv)
{
  int opt;
  int prevoptind;
  opterr = 0;

  parsebgp_opts_t opts;
  parsebgp_opts_init(&opts);

  while (prevoptind = optind, (opt = getopt(argc, argv, ":f:t:i4bsmqvh?")) >= 0) {
    if (optind == prevoptind + 2 && (optarg == NULL || *optarg == '-')) {
      opt = ':';
      --optind;
    }
    switch (opt) {
    case '4':
      opts.bgp.asn_4_byte = 1;
      break;

    case 'b':
      opts.bmp.parse_headers_only = 1;
      break;

    case 'f':
      opts.bgp.path_attr_filter_enabled = 1;
      opts.bgp.path_attr_filter[(uint8_t)atoi(optarg)] = 1;
      fprintf(stderr, "INFO: Filtering to include UPDATE Path Attribute %d\n",
              (uint8_t)atoi(optarg));
      break;

    case 'i':
      // if this is the second (or more) time, silence the warnings
      if (opts.ignore_invalid) {
        opts.silence_invalid = 1;
      }
      opts.ignore_invalid = 1;
      break;

    case 's':
      // if this is the second (or more) time, silence the warnings
      if (opts.ignore_not_implemented) {
        opts.silence_not_implemented = 1;
      }
      opts.ignore_not_implemented = 1;
      break;

    case 'm':
      opts.bgp.marker_omitted = 1;
      break;

    case 'q':
      silent = 1;
      break;

    case 'h':
    case '?':
      usage();
      return 0;
      break;

    case 'v':
      fprintf(stderr, "libparsebgp version %d.%d.%d\n",
              LIBPARSEBGP_MAJOR_VERSION, LIBPARSEBGP_MID_VERSION,
              LIBPARSEBGP_MINOR_VERSION);
      break;

    default:
      usage();
      return -1;
      break;
    }
  }

  if (optind >= argc) {
    usage();
    return -1;
  }

  int i, j;
  for (i = optind; i < argc; i++) {
    int type = 0; // undefined type
    char *fname, *tname, *freeme;
    fname = tname = freeme = strdup(argv[i]);
    assert(fname != NULL);

    if ((fname = strchr(fname, ':')) == NULL) {
      fname = tname;
      int len = strlen(fname);
      PARSEBGP_FOREACH_MSG_TYPE(j)
      {
        tname = fname;
        tname += (len - strlen(type_strs[j]));
        if (strcmp(tname, type_strs[j]) == 0) {
          type = j;
          break;
        }
      }
    } else {
      *(fname++) = '\0';
      PARSEBGP_FOREACH_MSG_TYPE(j)
      {
        if (strcmp(tname, type_strs[j]) == 0) {
          type = j;
          break;
        }
      }
    }

    if (type == 0) {
      fprintf(stderr,
              "ERROR: Could not identify type of %s, "
              "consider explicitly specifying type using type:file syntax\n",
              argv[i]);
      usage();
      free(freeme);
      return -1;
    }

    fprintf(stderr, "INFO: Parsing %s (Type: %s)\n", fname, type_strs[type]);

    if (parse(&opts, type, fname) != 0) {
      fprintf(stderr, "WARNING: Failed to parse %s%s\n", fname,
              (i == argc - 1) ? "" : ", moving on");
    }
    free(freeme);
  }

  return 0;
}
