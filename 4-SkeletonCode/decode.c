#include "common.h"
#include "decode.h"
#include "logs.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo) {

    LOG_INFO(INFO_CHECK_ARGUMENTS);
    if (strstr(argv[2], ".bmp") == NULL) {
        LOG_ERROR(ERR_INVALID_BMP);
        return e_failure;
    }
    decInfo->src_image_fname = argv[2];

    if (argv[3] && strstr(argv[3], ".") != NULL) {
        LOG_ERROR(ERR_INVALID_OUTPUT);
        return e_failure;
    }

    else if (argv[3])
        decInfo->output_fname = argv[3];
    else
        decInfo->output_fname = "output";

    return e_success;
}

Status open_decode_files(DecodeInfo *decInfo) {
    LOG_INFO(INFO_OPEN_FILES);
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");

    if (decInfo->fptr_src_image == NULL) {
        return e_failure;
    }
    return e_success;
}

Status skip_bmp_header(FILE *fptr_src_image) {
    LOG_INFO(INFO_COPY_HEADER);

    if (fseek(fptr_src_image, BMP_HEADER_SIZE, SEEK_SET) != 0)
        return e_failure;

    return e_success;
}

Status decode_byte_from_lsb(char *ptr_data, unsigned char *image_buffer) {
    LOG_INFO(INFO_ENCODING_START);

    unsigned char data = 0;
    for (int i = 0; i < 8; i++) {
        data <<= 1;
        data |= image_buffer[i] & 1;
    }

    *ptr_data = data;

    return e_success;
}

Status decode_data_from_image(char *data, int size, FILE *fptr_src_image) {
    LOG_INFO(INFO_ENCODING_START);

    for (int i = 0; i < size; i++) {
        unsigned char image_buffer[8];

        if (fread(image_buffer, sizeof(image_buffer), 1, fptr_src_image) != 1)
            return e_failure;

        decode_byte_from_lsb(&data[i], image_buffer);
    }

    return e_success;
}
Status decode_magic_string(DecodeInfo *decInfo) {
    LOG_INFO(INFO_ENCODE_MAGIC);

    int magic_len = strlen(MAGIC_STRING);
    char *magic = malloc(magic_len);

    if (decode_data_from_image(magic, magic_len, decInfo->fptr_src_image) ==
        e_failure)
        return e_failure;

    printf("enter the magic string :-> ");

    char user_magic[magic_len];
    scanf("%s", user_magic);

    if (strcmp((char *)magic, user_magic)) {
        return e_failure;
    }
    printf("%s matches the magic string", magic);
    free(magic);

    return e_success;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo) {
    LOG_INFO(INFO_ENCODE_EXTENSION_SIZE);

    if (decode_data_from_image(&decInfo->size_extn_output_file, 1,
                               decInfo->fptr_src_image) == e_failure) {
        return e_failure;
    }

    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo) {
    LOG_INFO(INFO_CHECK_SECRET_SIZE);

    char *output_file = malloc(strlen(decInfo->output_fname) +
                               decInfo->size_extn_output_file + 1);

    strcpy(output_file, decInfo->output_fname);
    char *extn_output_file = malloc(decInfo->size_extn_output_file + 1);

    if (decode_data_from_image(extn_output_file, decInfo->size_extn_output_file,
                               decInfo->fptr_src_image) == e_failure) {
        return e_failure;
    }

    extn_output_file[(int)decInfo->size_extn_output_file] = '\0';
    strcat(output_file, extn_output_file);
    free(extn_output_file);

    decInfo->output_fname = output_file;
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo) {
    LOG_INFO(INFO_ENCODE_FILE_SIZE);

    char *file_size = malloc(sizeof(int));

    if (decode_data_from_image(file_size, 4, decInfo->fptr_src_image)) {
        return e_failure;
    }
    decInfo->size_output_file = *(uint *)file_size;

    free(file_size);

    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo) {
    LOG_INFO(INFO_ENCODE_FILE_SIZE);

    char *file_data = malloc(decInfo->size_output_file);

    if (decode_data_from_image(file_data, decInfo->size_output_file,
                               decInfo->fptr_src_image) == e_failure) {
        return e_failure;
    }

    decInfo->fptr_output = fopen(decInfo->output_fname, "w");

    if (!decInfo->fptr_output) {
        LOG_ERROR(ERR_OPEN_FILES);
        return e_failure;
    }

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

    free(decInfo->output_fname);

    return e_success;
}

Status do_decoding(DecodeInfo *decInfo) {
    if (open_decode_files(decInfo) == e_failure) {
        LOG_ERROR(ERR_OPEN_FILES);
        return e_failure;
    }

    if (skip_bmp_header(decInfo->fptr_src_image) == e_failure) {
        LOG_ERROR(ERR_COPY_HEADER);
        return e_failure;
    }

    if (decode_magic_string(decInfo) == e_failure) {
        LOG_ERROR(ERR_ENCODE_MAGIC);
        return e_failure;
    }

    if (decode_secret_file_extn_size(decInfo) == e_failure) {
        LOG_ERROR(ERR_ENCODE_EXTENSION_SIZE);
        return e_failure;
    }

    if (decode_secret_file_extn(decInfo) == e_failure) {
        LOG_ERROR(ERR_ENCODE_EXTENSION);
        return e_failure;
    }

    if (decode_secret_file_size(decInfo) == e_failure) {
        LOG_ERROR(ERR_ENCODE_FILE_SIZE);
        return e_failure;
    }

    if (decode_secret_file_data(decInfo) == e_failure) {
        LOG_ERROR(ERR_ENCODE_FILE_DATA);
        return e_failure;
    }

    if (close_decode_files(decInfo) == e_failure) {
        LOG_ERROR(ERR_CLOSE_FILES);
        return e_failure;
    }
    return e_success;
}
