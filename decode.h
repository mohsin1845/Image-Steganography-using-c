#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * decoding secret file from stego Image
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4


typedef struct _DecodeInfo
{
    //stego file info
    char *stego_fname;
    FILE  *fptr_stego;
    char file_extn[MAX_FILE_SUFFIX];
    char extn_fname[MAX_SECRET_BUF_SIZE];

    //output file
    char *output_fname;
    FILE *fptr_output;
    char secret_data[8];
    long length_secret_data;


}DecodeInfo;


/* Decoding function prototype */

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for o/p file */
Status open_files_decoding(DecodeInfo *decInfo);

/* Decode Magic String */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);

/* Decode a byte into LSB of o/p image data */
Status decode_byte_from_lsb(char magic, char *data);

/* Decode secret file extension size */
Status decode_secret_extension_file_size(int size, FILE *fptr_stego, DecodeInfo *decInfo);

/* Decode secret file extension length*/
Status decode_size_from_lsb(char *str, int size);

/* Decode secret file extension */
Status decode_secret_file_extn(char *file_extn, DecodeInfo *decInfo);

/* Decode secret file extension size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/*Decode o/p image data from LSB*/
Status decode_data_from_lsb_stego(char *data, char *str, int k);

#endif
