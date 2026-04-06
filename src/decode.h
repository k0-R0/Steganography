#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types
#include <stdio.h>

/*
 * Structure to store information required for
 * decoding secret file from source Image
 */

typedef struct _DecodeInfo {
    /* Encoded (Source) Image info */
    char *src_image_fname;
    FILE *fptr_src_image;

    /* Decoded File Info */
    char *output_fname;
    FILE *fptr_output;
    char size_extn_output_file;
    uint size_output_file;

} DecodeInfo;

/* Encoding function prototype */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *decInfo);

/* Skip bmp image header */
Status skip_bmp_header(FILE *fptr_src_image);

/* Decode Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode secret file extenstion size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode function, which does the real decoding */
Status decode_data_from_image(char *data, int size, FILE *fptr_src_image);

/* Decode a byte into LSB of image data array */
Status decode_byte_from_lsb(char *ptr_data, unsigned char *image_buffer);

/* Close all files while exiting*/
Status close_decode_files(DecodeInfo *decInfo);

#endif
