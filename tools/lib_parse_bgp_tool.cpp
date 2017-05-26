#include <iostream>
#include <fstream>
#include "../include/lib_parse_common.h"

using namespace std;

int read_file(ifstream &fin, u_char *array2, int position, int read_size) {
    int curr=0;
    char *array = new char[read_size];
    if(fin.is_open())
    {
        fin.seekg(position, fin.beg);
        //fin.read(array, read_size);
        while(!fin.eof() && curr < read_size)
        {
            fin.get(array[curr]); //reading one character from file to array
            curr++;
        }
        array[curr-1] = '\0'; //placing character array terminating character

        for(int i=0, j=0;i<read_size;i+=3, j++)
        {
            int tmp;
            sscanf(array+i, "%2x",&tmp);
            array2[j]=tmp;
        }
        cout<<"file opened successfully\n";
    }
    else //file could not be opened
        cout << "File could not be opened." << endl;

    return fin.eof();
}

//int main() {
//    libparsebgp_parse_msg parse_msg;
//
//    u_char temp[] = {0x58, 0xb6, 0x0f, 0x00, 0x00, 0x0d, 0x00, 0x02, 0x00, 0x00, 0x00, 0xcd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//                     0x05, 0x00, 0x14, 0x58, 0xb5, 0xe3, 0x61, 0x00, 0x27, 0x40, 0x01, 0x01, 0x00, 0x50, 0x02, 0x00, 0x0a, 0x02,
//                     0x02, 0x00, 0x00, 0x85, 0xb0, 0x00, 0x00, 0x0d, 0xdd, 0x40, 0x03, 0x04, 0x5e, 0x9c, 0xfc, 0x12, 0x80, 0x04,
//                     0x04, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x08, 0x04, 0x85, 0xb0, 0x01, 0x4d, 0x00, 0x2a, 0x58, 0xaf, 0x4f, 0x3b,
//                     0x00, 0x30, 0x40, 0x01, 0x01, 0x00, 0x50, 0x02, 0x00, 0x0a, 0x02, 0x02, 0x00, 0x00, 0xbb, 0x00, 0x00, 0x00,
//                     0x0d, 0x1c, 0x40, 0x03, 0x04, 0xb9, 0x2c, 0x74, 0x01, 0xc0, 0x08, 0x14, 0x0d, 0x1c, 0x00, 0x02, 0x0d, 0x1c,
//                     0x02, 0x02, 0x0d, 0x1c, 0x08, 0x43, 0xbb, 0x00, 0x00, 0x06, 0xbb, 0x00, 0x0d, 0x1c, 0x00, 0x13, 0x58, 0x91,
//                     0x2d, 0x7f, 0x00, 0x19, 0x40, 0x01, 0x01, 0x00, 0x50, 0x02, 0x00, 0x0a, 0x02, 0x02, 0x00, 0x00, 0x79, 0x2b,
//                     0x00, 0x00, 0x99, 0x9e, 0x40, 0x03, 0x04, 0x5b, 0xe4, 0x97, 0x01, 0x00, 0x34, 0x58, 0x90, 0x94, 0x4e, 0x00,
//                     0x15, 0x40, 0x01, 0x01, 0x00, 0x50, 0x02, 0x00, 0x06, 0x02, 0x01, 0x00, 0x00, 0x46, 0xba, 0x40, 0x03, 0x04,
//                     0xca, 0x49, 0x28, 0x2d, 0x00, 0x0e, 0x58, 0xb5, 0x9c, 0xbd, 0x00, 0x19, 0x40, 0x01, 0x01, 0x00, 0x50, 0x02,
//                     0x00, 0x0a, 0x02, 0x02, 0x00, 0x00, 0x51, 0x23, 0x00, 0x00, 0x0d, 0x1c, 0x40, 0x03, 0x04, 0x50, 0xf1, 0xb0,
//                     0x1f};
//
//    uint32_t size = 217;
//    ssize_t read_size;
//    read_size=libparsebgp_parse_msg_common_wrapper(parse_msg, temp, size, 1);
//    cout << "Message Parsed Successfully"<<endl;
//
//    cout<<read_size<<"\n" << int(parse_msg.parsed_bmp_msg.libparsebgp_parsed_bmp_hdr.c_hdr_v3.ver) << "\n"<<parse_msg.parsed_bmp_msg.libparsebgp_parsed_bmp_hdr.c_hdr_v3.len<<endl;
//    cout<<int(parse_msg.parsed_bmp_msg.libparsebgp_parsed_bmp_hdr.c_hdr_v3.type)<<endl;
//    return 1;
//}

int main() {
    string filename = "../testfile.txt";
    int msg_type = 2;
//    for (int i=1; i < argc; i++) {
//        if (!strcmp(argv[i], "-c")) {
//            // We expect the next arg to be the filename
//            if (i + 1 >= argc) {
//                cout << "INVALID ARG: -c expects the filename to be specified" << endl;
//                return true;
//            }
//
//            // Set the new filename
//            filename = argv[++i];
//        }
//        else if (!strcmp(argv[i], "-t")) {
//            // We expect the next arg to be the type of message
//            if (i + 1 >= argc) {
//                cout << "INVALID ARG: -t expects the type to be specified" << endl;
//                return true;
//            }
//
//            // Set the message type
//            msg_type = argv[++i];
//        }
//    }

    int read_size = 3072;
    int position = 0;
    bool msg_read = true;
    int len = 1024, cur=0, bytes_read;
    int end_reach=0;

    ifstream fin(filename);

    while(!end_reach)
    {
        cur++;
        u_char *buffer = new u_char[read_size/3];
        end_reach = read_file(fin, buffer, 3*position, read_size);
        msg_read = true;
        len = 1024;
        while(msg_read)
        {
            //parseBMP *p = new parseBMP();
            libparsebgp_parse_msg *parse_msg = malloc(sizeof(libparsebgp_parse_msg));
            memset(parse_msg, 0, sizeof(libparsebgp_parse_msg));

            bytes_read=libparsebgp_parse_msg_common_wrapper(parse_msg, buffer, len, msg_type);
            cout << "Message Parsed Successfully"<<endl;

            if(bytes_read < 0) {
                msg_read = false;
                cout << "Crashed. Error code: " << bytes_read << endl;
            }
            else if (bytes_read == 0)
                msg_read=false;
            else {
                position += bytes_read;
                cout << bytes_read << " " << position << endl;
                buffer += bytes_read;
            }
        }
        if(cur==2)
            return 1;
    }
    return 1;
}

//unsigned char temp[] = {0x03, 0x00, 0x00, 0x00, 0x06, 0x04};
//len = 186
//unsigned char temp[] = {0x03, 0x00, 0x00, 0x00, 0xba, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x45, 0xb8, 0xc1, 0x00, 0x00, 0x0d, 0x1c, 0x04, 0x45, 0xb8, 0xc1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xdf, 0x33, 0x67, 0xd0, 0x40, 0x00, 0xb3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x41, 0x01, 0x04, 0x19, 0x2f, 0x02, 0x58, 0x80, 0xdf, 0x33, 0x67, 0x24, 0x02, 0x06, 0x01, 0x04, 0x00, 0x01, 0x00, 0x02, 0x02, 0x06, 0x01, 0x04, 0x00, 0x01, 0x00, 0x01, 0x02, 0x02, 0x80, 0x00, 0x02, 0x02, 0x02, 0x00, 0x02, 0x02, 0x46, 0x00, 0x02, 0x06, 0x41, 0x04, 0x00, 0x00, 0x19, 0x2f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x35, 0x01, 0x04, 0x0d, 0x1c, 0x00, 0xb4, 0x04, 0x45, 0xb8, 0xc1, 0x18, 0x02, 0x06, 0x01, 0x04, 0x00, 0x01, 0x00, 0x02, 0x02, 0x06, 0x01, 0x04, 0x00, 0x01, 0x00, 0x01, 0x02, 0x02, 0x02, 0x00, 0x02, 0x02, 0x80, 0x00};
//len = 228
//unsigned char temp[] = {0x03, 0x00, 0x00, 0x00, 0xe4, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x45, 0xb8, 0xc1, 0x00, 0x00, 0x0d, 0x1c, 0x04, 0x45, 0xb8, 0xc1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x92, 0x03, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x92, 0x03, 0x7f, 0xff, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe8, 0x80, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xeb, 0x80, 0x01, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x02, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xeb, 0x80, 0x03, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x80, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe8, 0x80, 0x05, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0x80, 0x06, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0xb4};
//len = 870
//unsigned char temp[] = {0x03, 0x00, 0x00, 0x03, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x45, 0xb8, 0xc1, 0x00, 0x00, 0x0d, 0x1c, 0x04, 0x45, 0xb8, 0xc1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x36, 0x02, 0x00, 0x00, 0x00, 0x44, 0x40, 0x01, 0x01, 0x00, 0x40, 0x02, 0x08, 0x02, 0x03, 0x0d, 0x1c, 0x0b, 0x62, 0x40, 0x7d, 0x40, 0x03, 0x04, 0x04, 0x45, 0xb8, 0xc1, 0x80, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x08, 0x24, 0x0b, 0x62, 0x01, 0x9a, 0x0b, 0x62, 0x04, 0xb3, 0x0b, 0x62, 0x08, 0x99, 0x0b, 0x62, 0x0c, 0x80, 0x0d, 0x1c, 0x00, 0x03, 0x0d, 0x1c, 0x00, 0x56, 0x0d, 0x1c, 0x02, 0x3f, 0x0d, 0x1c, 0x02, 0x9a, 0x0d, 0x1c, 0x07, 0xdc, 0x17, 0x36, 0xe7, 0x82, 0x16, 0x34, 0xda, 0x40, 0x15, 0x34, 0xda, 0x38, 0x16, 0x34, 0xda, 0x34, 0x18, 0x36, 0xb6, 0xf4, 0x18, 0x34, 0xde, 0xf2, 0x17, 0x34, 0xde, 0xf0, 0x18, 0x34, 0xde, 0xef, 0x17, 0x34, 0xde, 0xec, 0x17, 0x34, 0xde, 0xe8, 0x16, 0x34, 0xde, 0xe4, 0x17, 0x34, 0xde, 0xe2, 0x18, 0x34, 0x54, 0x49, 0x18, 0x36, 0xef, 0x27, 0x18, 0x36, 0xef, 0x25, 0x18, 0x36, 0xef, 0x22, 0x18, 0x36, 0xef, 0x20, 0x18, 0x36, 0xe7, 0x9f, 0x18, 0x36, 0xe7, 0x9e, 0x18, 0x36, 0xe7, 0x9d, 0x18, 0x36, 0xe7, 0x9a, 0x18, 0x36, 0xe7, 0x99, 0x18, 0x36, 0xe7, 0x98, 0x18, 0x36, 0xe7, 0x97, 0x18, 0x36, 0xe7, 0x94, 0x18, 0x36, 0xe7, 0x93, 0x18, 0x36, 0xe7, 0x92, 0x18, 0x36, 0xe7, 0x91, 0x18, 0x36, 0xe7, 0x8e, 0x18, 0x36, 0xe7, 0x8d, 0x18, 0x36, 0xe7, 0x8b, 0x18, 0x36, 0xe7, 0x8a, 0x18, 0x36, 0xe7, 0x89, 0x18, 0x36, 0xe7, 0x87, 0x18, 0x36, 0xe7, 0x86, 0x18, 0x36, 0xe7, 0x84, 0x18, 0x36, 0xe7, 0x83, 0x18, 0x36, 0xe7, 0x81, 0x13, 0x36, 0xe7, 0x80, 0x18, 0x36, 0xe7, 0x80, 0x16, 0x36, 0xe6, 0x1c, 0x11, 0x36, 0xe5, 0x80, 0x11, 0x36, 0xe5, 0x00, 0x10, 0x36, 0xe4, 0x10, 0x36, 0xdc, 0x0f, 0x36, 0xd8, 0x10, 0x36, 0xc3, 0x10, 0x36, 0xc2, 0x16, 0x36, 0xc0, 0x1c, 0x17, 0x36, 0xb6, 0xf0, 0x18, 0x36, 0xb6, 0xc8, 0x18, 0x36, 0xb6, 0xc7, 0x18, 0x36, 0xb6, 0xc6, 0x17, 0x36, 0xb6, 0x90, 0x17, 0x36, 0xb6, 0x8e, 0x17, 0x36, 0xb6, 0x8c, 0x10, 0x36, 0xab, 0x10, 0x36, 0xaa, 0x10, 0x36, 0x9b, 0x10, 0x36, 0x9a, 0x10, 0x36, 0x4e, 0x0f, 0x36, 0x4c, 0x0f, 0x36, 0x4a, 0x10, 0x36, 0x49, 0x10, 0x36, 0x48, 0x18, 0x34, 0xda, 0x4f, 0x18, 0x34, 0xda, 0x4e, 0x18, 0x34, 0xda, 0x4b, 0x18, 0x34, 0xda, 0x4a, 0x18, 0x34, 0xda, 0x49, 0x18, 0x34, 0xda, 0x48, 0x18, 0x34, 0xda, 0x45, 0x18, 0x34, 0xda, 0x44, 0x18, 0x34, 0xda, 0x43, 0x18, 0x34, 0xda, 0x42, 0x18, 0x34, 0xda, 0x3f, 0x18, 0x34, 0xda, 0x3c, 0x18, 0x34, 0xda, 0x3b, 0x18, 0x34, 0xda, 0x3a, 0x18, 0x34, 0xda, 0x39, 0x18, 0x34, 0xda, 0x36, 0x18, 0x34, 0xda, 0x35, 0x18, 0x34, 0xda, 0x34, 0x18, 0x34, 0xda, 0x33, 0x18, 0x34, 0xda, 0x30, 0x18, 0x34, 0xda, 0x2d, 0x18, 0x34, 0xda, 0x2c, 0x18, 0x34, 0xda, 0x2b, 0x18, 0x34, 0xda, 0x2a, 0x18, 0x34, 0xda, 0x27, 0x18, 0x34, 0xda, 0x26, 0x18, 0x34, 0xda, 0x25, 0x18, 0x34, 0xda, 0x24, 0x18, 0x34, 0xda, 0x21, 0x18, 0x34, 0xda, 0x20, 0x18, 0x34, 0xda, 0x1e, 0x18, 0x34, 0xda, 0x1d, 0x18, 0x34, 0xda, 0x1c, 0x18, 0x34, 0xda, 0x1b, 0x18, 0x34, 0xda, 0x18, 0x18, 0x34, 0xda, 0x17, 0x18, 0x34, 0xda, 0x16, 0x18, 0x34, 0xda, 0x15, 0x18, 0x34, 0xda, 0x12, 0x18, 0x34, 0xda, 0x11, 0x18, 0x34, 0xda, 0x10, 0x18, 0x34, 0xda, 0x0f, 0x18, 0x34, 0xda, 0x0e, 0x18, 0x34, 0xda, 0x0d, 0x18, 0x34, 0xda, 0x0c, 0x18, 0x34, 0xda, 0x09, 0x18, 0x34, 0xda, 0x08, 0x18, 0x34, 0xda, 0x07, 0x18, 0x34, 0xda, 0x06, 0x18, 0x34, 0xda, 0x03, 0x18, 0x34, 0xda, 0x02, 0x18, 0x34, 0xda, 0x01, 0x11, 0x34, 0xda, 0x00, 0x18, 0x34, 0xda, 0x00, 0x0d, 0x34, 0xd0, 0x18, 0x34, 0x5f, 0xfd, 0x18, 0x34, 0x5f, 0xf4, 0x18, 0x34, 0x5f, 0x96, 0x18, 0x34, 0x5f, 0x95, 0x17, 0x34, 0x5f, 0x94, 0x18, 0x34, 0x5f, 0x94, 0x16, 0x34, 0x5f, 0x68, 0x15, 0x34, 0x5e, 0xd8, 0x16, 0x34, 0x5e, 0x70, 0x14, 0x34, 0x5e, 0x30, 0x14, 0x34, 0x5e, 0x20, 0x17, 0x34, 0x5e, 0x18, 0x18, 0x34, 0x5e, 0x0f, 0x18, 0x34, 0x5e, 0x05, 0x18, 0x34, 0x5c, 0x5b, 0x18, 0x34, 0x5c, 0x5a, 0x18, 0x34, 0x5c, 0x59, 0x16, 0x34, 0x5c, 0x58, 0x18, 0x34, 0x5c, 0x58, 0x15, 0x34, 0x5c, 0x28, 0x18, 0x34, 0x55, 0xc6, 0x17, 0x34, 0x55, 0xc4, 0x10, 0x34, 0x38, 0x0e, 0x34, 0x30, 0x0f, 0x34, 0x1e, 0x0f, 0x34, 0x12, 0x0f, 0x34, 0x10, 0x12, 0x2e, 0x89, 0x80, 0x11, 0x2e, 0x89, 0x00, 0x14, 0x2e, 0x33, 0xc0, 0x12, 0x2e, 0x33, 0x80, 0x18, 0xd8, 0x89, 0x39, 0x18, 0xd8, 0x89, 0x38, 0x18, 0xcc, 0xf6, 0xbd, 0x18, 0xb9, 0x8f, 0x10, 0x16, 0xb9, 0x30, 0x78, 0x14, 0xb2, 0xec, 0x00, 0x12, 0xb0, 0x22, 0xc0, 0x13, 0xb0, 0x22, 0xa0, 0x14, 0xb0, 0x22, 0x90, 0x14, 0xb0, 0x22, 0x80, 0x12, 0xb0, 0x22, 0x40, 0x15, 0xb0, 0x20, 0x68, 0x15, 0x57, 0xee, 0x50, 0x11, 0x4f, 0x7d, 0x00, 0x12, 0x4f, 0x7d, 0x00, 0x12, 0x36, 0xf7, 0xc0, 0x12, 0x36, 0xf7, 0x80, 0x11, 0x36, 0xf7, 0x00, 0x11, 0x36, 0xf6, 0x80, 0x11, 0x36, 0xf6, 0x00, 0x16, 0x36, 0xf0, 0xdc, 0x18, 0x36, 0xf0, 0xc5, 0x18, 0x36, 0xf0, 0x38, 0x16, 0x36, 0xf0, 0x34, 0x17, 0x36, 0xf0, 0x32, 0x15, 0x36, 0xf0, 0x00, 0x18, 0x36, 0xef, 0xdf, 0x18, 0x36, 0xef, 0xa6, 0x17, 0x36, 0xef, 0xa4, 0x18, 0x36, 0xef, 0x63, 0x15, 0x36, 0xef, 0x20, 0x16, 0x36, 0xe6, 0xc4, 0x16, 0x36, 0xc0, 0xc4, 0x18, 0x34, 0x55, 0x3f, 0x17, 0x34, 0x55, 0x3c, 0x17, 0x34, 0x55, 0x3a, 0x0d, 0x22, 0xf8};


//           cout<<len;
//           cout<<len<<" "<<int(*buffer)<<int(*(buffer+1))<<int(*(buffer+2))<<int(*(buffer+3))<<int(*(buffer+4))<<int(*(buffer+5))<<int(*(buffer+6))<<endl;
//            cout<<"Peer Address "<<p->p_entry.peer_addr<<" "<<p->p_entry.timestamp_secs<<" "<<p->p_entry.isPrePolicy<<endl;
//            cout<<p->bgpMsg.common_hdr.len<<" "<<int(p->bgpMsg.common_hdr.type)<<endl;
//            <dump-type>|<elem-type>|<record-ts>|<project>|<collector>|<peer-ASn>|<peer-IP>|<prefix>|<next-hop-IP>|<AS-path>|<origin-AS>|<communities>|<old-state>|<new-state>
//            if (p->getBMPType() == 0 || p->getBMPType() == 1 || p->getBMPType() == 3) {
//                stringstream out;
//                if (p->bgpMsg.hasEndOfRIBMarker)
//                    out << "R|E|"; //End of RIB
//                else
//                    out << "U||";
//                out << p->p_entry.timestamp_secs << "|||" << p->p_entry.peer_addr << "|" << p->p_entry.peer_as << "||";
//                if (p->bgpMsg.parsed_data.attrs.size() > 0)
//                    out << p->bgpMsg.parsed_data.attrs[parseBMP::ATTR_TYPE_NEXT_HOP] << "|" << p->bgpMsg.parsed_data.attrs[parseBMP::ATTR_TYPE_AS_PATH] << "|" << p->bgpMsg.parsed_data.attrs[parseBMP::ATTR_TYPE_ORIGIN] << "|" << p->bgpMsg.parsed_data.attrs[parseBMP::ATTR_TYPE_COMMUNITIES] << "|";
//                else
//                    out << "||||";
//                out << "|";
//                cout << out.str() << endl;
//            }
//            delete p;

//mrt
//len=99
//u_char temp[] = {0x58, 0xb6, 0x12, 0x84, 0x00, 0x10, 0x00, 0x04, 0x00, 0x00, 0x00, 0x57, 0x00, 0x00, 0xe4, 0x8f,
//                 0x00, 0x00, 0x19, 0x2f, 0x00, 0x00, 0x00, 0x01, 0x67, 0xf7, 0x03, 0x2d, 0x80, 0xdf, 0x33, 0x66,
//                 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
//                 0x00, 0x43, 0x02, 0x00, 0x04, 0x18, 0xa8, 0xb5, 0x24, 0x00, 0x24, 0x40, 0x01, 0x01, 0x00, 0x40,
//                 0x02, 0x16, 0x02, 0x05, 0x00, 0x00, 0xe4, 0x8f, 0x00, 0x00, 0x1b, 0x1b, 0x00, 0x04, 0x01, 0xbd,
//                 0x00, 0x00, 0x6e, 0xb7, 0x00, 0x04, 0x05, 0x73, 0x40, 0x03, 0x04, 0x67, 0xf7, 0x03, 0x2d, 0x18,
//                 0xbf, 0x05, 0xaa};

//len=75
/*u_char temp[] = {0x58, 0x67, 0xb5, 0x31, 0x00, 0x10, 0x00, 0x04, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x07, 0x2c,
                 0x00, 0x00, 0x31, 0x6e, 0x00, 0x00, 0x00, 0x02, 0x2a, 0x01, 0x02, 0xa8, 0x00, 0x00, 0x00, 0x00,
                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x20, 0x01, 0x06, 0x7c, 0x02, 0xe8, 0x00, 0x02,
                 0xff, 0xff, 0x00, 0x00, 0x00, 0x04, 0x00, 0x28, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x13, 0x04};*/

//u_char temp[] = {0x58, 0xb6, 0x0f, 0x00, 0x00, 0x0d, 0x00, 0x01, 0x00, 0x00, 0x03, 0x96, 0x80, 0xdf, 0x33, 0x66, 0x00, 0x00, 0x00, 0x46, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x8c, 0xc0, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x12, 0x1f, 0x66, 0x00, 0x00, 0x00, 0x00, 0x02, 0x04, 0x45, 0xb8, 0xc1, 0x04, 0x45, 0xb8, 0xc1, 0x00, 0x00, 0x0d, 0x1c, 0x02, 0x05, 0x65, 0x6e, 0x02, 0x05, 0x65, 0x6e, 0x02, 0x00, 0x03, 0x15, 0x22, 0x02, 0x0c, 0x00, 0x01, 0x3f, 0x0c, 0x00, 0x01, 0x3f, 0x00, 0x00, 0x1b, 0x6a, 0x02, 0x44, 0x43, 0x21, 0x63, 0x2d, 0x3d, 0x00, 0x55, 0x00, 0x00};

//len = 217
//u_char temp[] = {0x58, 0xb6, 0x0f, 0x00, 0x00, 0x0d, 0x00, 0x02, 0x00, 0x00, 0x00, 0xcd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//                 0x05, 0x00, 0x14, 0x58, 0xb5, 0xe3, 0x61, 0x00, 0x27, 0x40, 0x01, 0x01, 0x00, 0x50, 0x02, 0x00, 0x0a, 0x02,
//                 0x02, 0x00, 0x00, 0x85, 0xb0, 0x00, 0x00, 0x0d, 0xdd, 0x40, 0x03, 0x04, 0x5e, 0x9c, 0xfc, 0x12, 0x80, 0x04,
//                 0x04, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x08, 0x04, 0x85, 0xb0, 0x01, 0x4d, 0x00, 0x2a, 0x58, 0xaf, 0x4f, 0x3b,
//                 0x00, 0x30, 0x40, 0x01, 0x01, 0x00, 0x50, 0x02, 0x00, 0x0a, 0x02, 0x02, 0x00, 0x00, 0xbb, 0x00, 0x00, 0x00,
//                 0x0d, 0x1c, 0x40, 0x03, 0x04, 0xb9, 0x2c, 0x74, 0x01, 0xc0, 0x08, 0x14, 0x0d, 0x1c, 0x00, 0x02, 0x0d, 0x1c,
//                 0x02, 0x02, 0x0d, 0x1c, 0x08, 0x43, 0xbb, 0x00, 0x00, 0x06, 0xbb, 0x00, 0x0d, 0x1c, 0x00, 0x13, 0x58, 0x91,
//                 0x2d, 0x7f, 0x00, 0x19, 0x40, 0x01, 0x01, 0x00, 0x50, 0x02, 0x00, 0x0a, 0x02, 0x02, 0x00, 0x00, 0x79, 0x2b,
//                 0x00, 0x00, 0x99, 0x9e, 0x40, 0x03, 0x04, 0x5b, 0xe4, 0x97, 0x01, 0x00, 0x34, 0x58, 0x90, 0x94, 0x4e, 0x00,
//                 0x15, 0x40, 0x01, 0x01, 0x00, 0x50, 0x02, 0x00, 0x06, 0x02, 0x01, 0x00, 0x00, 0x46, 0xba, 0x40, 0x03, 0x04,
//                 0xca, 0x49, 0x28, 0x2d, 0x00, 0x0e, 0x58, 0xb5, 0x9c, 0xbd, 0x00, 0x19, 0x40, 0x01, 0x01, 0x00, 0x50, 0x02,
//                 0x00, 0x0a, 0x02, 0x02, 0x00, 0x00, 0x51, 0x23, 0x00, 0x00, 0x0d, 0x1c, 0x40, 0x03, 0x04, 0x50, 0xf1, 0xb0,
//                 0x1f};

