#include "common.h"
#include "decode.h"
#include "logs.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// validate decode args - 1st argument should be the bmp file
// where we expect to find some encoded data, next argument is optional
// and contains the output file we want to store the encoded file back into
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo) {

    LOG_INFO(INFO_CHECK_ARGUMENTS);
    // get extension of encoded file and validate
    char *src_fname_extn = strstr(argv[2], ".bmp");
    if (src_fname_extn == NULL || strcmp(src_fname_extn, ".bmp")) {
        LOG_ERROR(ERR_INVALID_BMP);
        return e_failure;
    }
    // store in structure if valid
    decInfo->src_image_fname = argv[2];

    // store in structure if present else default
    if (argv[3]) {
        decInfo->output_fname = strtok(argv[3], ".");
    } else
        decInfo->output_fname = "output";

    return e_success;
}

// open the file after validating that it's a bmp file
Status open_decode_files(DecodeInfo *decInfo) {
    LOG_INFO(INFO_OPEN_FILES);
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "rb");

    if (decInfo->fptr_src_image == NULL) {
        return e_failure;
    }
    return e_success;
}

// skip the header section of the bmp file because our encoded
// data is present after it
Status skip_bmp_header(FILE *fptr_src_image) {
    LOG_INFO(INFO_SKIP_HEADER);

    if (fseek(fptr_src_image, BMP_HEADER_SIZE, SEEK_SET) != 0)
        return e_failure;

    return e_success;
}

// decodes 1 byte of data from the buffer passed
Status decode_byte_from_lsb(char *ptr_data, unsigned char *image_buffer) {
    unsigned char data = 0;
    for (int i = 0; i < 8; i++) {
        data <<= 1;
        data |= image_buffer[i] & 1;
    }

    *ptr_data = data;

    return e_success;
}

// decodes size bytes of data from the file and stores it in data
Status decode_data_from_image(char *data, int size, FILE *fptr_src_image) {

    for (int i = 0; i < size; i++) {
        unsigned char image_buffer[8];

        if (fread(image_buffer, sizeof(image_buffer), 1, fptr_src_image) != 1)
            return e_failure;

        decode_byte_from_lsb(&data[i], image_buffer);
    }

    LOG_INFO(INFO_DONE);
    return e_success;
}

// decodes the magic string and checks if the user given string
// matches the magic string if it doesn't then this file isn't encoded
Status decode_magic_string(DecodeInfo *decInfo) {
    LOG_INFO(INFO_DECODE_MAGIC);

    int magic_len = strlen(MAGIC_STRING);
    char *magic = malloc(magic_len + 1);

    if (decode_data_from_image(magic, magic_len, decInfo->fptr_src_image) ==
        e_failure)
        return e_failure;

    magic[magic_len] = '\0';
    printf("enter the magic string :\n");

    // read user given magic
    char user_magic[magic_len + 1];
    scanf("%s", user_magic);

    if (strcmp(magic, user_magic)) {
        return e_failure;
    }

    free(magic);
    return e_success;
}

// decode secret file extension size
Status decode_secret_file_extn_size(DecodeInfo *decInfo) {
    LOG_INFO(INFO_DECODE_EXTENSION_SIZE);

    if (decode_data_from_image(&decInfo->size_extn_output_file, 1,
                               decInfo->fptr_src_image) == e_failure) {
        return e_failure;
    }

    return e_success;
}

// decode secret file extension
Status decode_secret_file_extn(DecodeInfo *decInfo) {
    LOG_INFO(INFO_DECODE_EXTENSION);

    // malloc an array to store final output file name
    // extension_size + output_fname_length + 1(null character)
    char *output_file = malloc(strlen(decInfo->output_fname) +
                               decInfo->size_extn_output_file + 1);

    strcpy(output_file, decInfo->output_fname);
    char *extn_output_file = malloc(decInfo->size_extn_output_file + 1);

    if (decode_data_from_image(extn_output_file, decInfo->size_extn_output_file,
                               decInfo->fptr_src_image) == e_failure) {
        return e_failure;
    }

    // after storing file extension name add null character at the end
    extn_output_file[(int)decInfo->size_extn_output_file] = '\0';
    strcat(output_file, extn_output_file);

    decInfo->output_fname = output_file;
    // free extension name as we no longer need it
    free(extn_output_file);
    return e_success;
}

// decode secret file size
Status decode_secret_file_size(DecodeInfo *decInfo) {
    LOG_INFO(INFO_DECODE_FILE_SIZE);

    char *file_size = malloc(sizeof(int));

    if (decode_data_from_image(file_size, 4, decInfo->fptr_src_image)) {
        return e_failure;
    }
    // store file size in structure
    decInfo->size_output_file = *(uint *)file_size;

    free(file_size);

    return e_success;
}

// decode file data
Status decode_secret_file_data(DecodeInfo *decInfo) {
    LOG_INFO(INFO_DECODE_FILE_DATA);

    // malloc a buffer that is file data size
    // decode encoded file into buffer
    char *file_data = malloc(decInfo->size_output_file);

    if (decode_data_from_image(file_data, decInfo->size_output_file,
                               decInfo->fptr_src_image) == e_failure) {
        return e_failure;
    }

    decInfo->fptr_output = fopen(decInfo->output_fname, "wb");

    if (!decInfo->fptr_output) {
        LOG_ERROR(ERR_OPEN_FILES);
        return e_failure;
    }

    // write buffer data to final output file
    if (fwrite(file_data, decInfo->size_output_file, 1, decInfo->fptr_output) !=
        1) {
        return e_failure;
    }

    free(file_data);

    return e_success;
}

Status close_decode_files(DecodeInfo *decInfo) {
    LOG_INFO(INFO_CLOSE_FILES);

    if (fclose(decInfo->fptr_output) == EOF)
        return e_failure;

    if (fclose(decInfo->fptr_src_image) == EOF)
        return e_failure;

    // free the output_fname pointer that was malloced earlier
    LOG_OUTPUT(decInfo->output_fname);
    free(decInfo->output_fname);

    return e_success;
}

// do decoding calls all the above in order
Status do_decoding(DecodeInfo *decInfo) {
    if (open_decode_files(decInfo) == e_failure) {
        LOG_ERROR(ERR_OPEN_FILES);
        return e_failure;
    }

    if (skip_bmp_header(decInfo->fptr_src_image) == e_failure) {
        LOG_ERROR(ERR_SKIP_HEADER);
        return e_failure;
    }

    LOG_INFO(INFO_DECODING_START);
    if (decode_magic_string(decInfo) == e_failure) {
        LOG_ERROR(ERR_DECODE_MAGIC);
        return e_failure;
    }

    if (decode_secret_file_extn_size(decInfo) == e_failure) {
        LOG_ERROR(ERR_DECODE_EXTENSION_SIZE);
        return e_failure;
    }

    if (decode_secret_file_extn(decInfo) == e_failure) {
        LOG_ERROR(ERR_DECODE_EXTENSION);
        return e_failure;
    }

    if (decode_secret_file_size(decInfo) == e_failure) {
        LOG_ERROR(ERR_DECODE_FILE_SIZE);
        return e_failure;
    }

    if (decode_secret_file_data(decInfo) == e_failure) {
        LOG_ERROR(ERR_DECODE_FILE_DATA);
        return e_failure;
    }

    if (close_decode_files(decInfo) == e_failure) {
        LOG_ERROR(ERR_CLOSE_FILES);
        return e_failure;
    }
    return e_success;
}
