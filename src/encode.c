#include "common.h"
#include "encode.h"
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
uint get_image_size_for_bmp(FILE *fptr_image) {
    LOG_INFO(INFO_CHECK_RGB_DATA_SIZE);
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    rewind(fptr_image);
    return width * height * 3;
}

uint get_file_size(FILE *fptr) {
    LOG_INFO(INFO_CHECK_SECRET_SIZE);
    fseek(fptr, 0, SEEK_END);
    uint size = ftell(fptr);
    rewind(fptr);
    return size;
}
/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_encode_files(EncodeInfo *encInfo) {
    // Src Image file
    LOG_INFO(INFO_OPEN_FILES);
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL) {
        perror("fopen");
        LOG_ERROR(ERR_OPEN_BMP);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL) {
        perror("fopen");
        LOG_ERROR(ERR_OPEN_SECRET);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL) {
        perror("fopen");
        LOG_ERROR(ERR_OPEN_OUTPUT);
        return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status read_and_validate_encode_args(char **argv, EncodeInfo *encInfo) {
    // check .bmp file
    LOG_INFO(INFO_CHECK_ARGUMENTS);

    char *src_fname_extn = strstr(argv[2], ".bmp");
    if (src_fname_extn == NULL || strcmp(src_fname_extn, ".bmp")) {
        LOG_ERROR(ERR_INVALID_BMP);
        return e_failure;
    }
    encInfo->src_image_fname = argv[2];

    // check . present in secret file
    if (strchr(argv[3], '.') == NULL) {
        LOG_ERROR(ERR_INVALID_SECRET);
        return e_failure;
    }
    encInfo->secret_fname = argv[3];
    encInfo->extn_secret_file = strstr(argv[3], ".");

    // check if output file is present and validate
    // if not default
    if (argv[4]) {
        char *steg_fname_extn = strstr(argv[4], ".bmp");
        if (steg_fname_extn == NULL || strcmp(steg_fname_extn, ".bmp")) {
            LOG_ERROR(ERR_INVALID_OUTPUT);
            return e_failure;
        }
        encInfo->stego_image_fname = argv[4];
    } else
        encInfo->stego_image_fname = "stego_img.bmp";

    return e_success;
}

Status check_capacity(EncodeInfo *encInfo) {
    // compare rgb data size to encoded text size
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    uint secret_size = encInfo->size_secret_file + 4;
    uint file_size = get_image_size_for_bmp(encInfo->fptr_src_image);

    // encoded text size =
    // magic string size + 1 unsigned char to represent magic string

    secret_size += strlen(MAGIC_STRING) + 1;
    // extension size + 1 unsigned char to represent extension size

    char *extension =
        &encInfo->secret_fname[strcspn(encInfo->secret_fname, ".")];

    uint extensionLen = strlen(extension);
    secret_size += extensionLen + 1;
    // file size + 1 int (4 bytes) to represent file size

    LOG_INFO(INFO_CHECK_CAPACITY);
    if (file_size < secret_size * 8) {
        return e_failure;
    }
    LOG_INFO(INFO_CAPACITY_OK);
    return e_success;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image) {
    LOG_INFO(INFO_COPY_HEADER);
    char header_buffer[BMP_HEADER_SIZE];
    if (fread(header_buffer, sizeof(char), BMP_HEADER_SIZE, fptr_src_image) <
        BMP_HEADER_SIZE) {
        return e_failure;
    }
    if (fwrite(header_buffer, sizeof(char), BMP_HEADER_SIZE, fptr_dest_image) <
        BMP_HEADER_SIZE) {
        return e_failure;
    }
    return e_success;
}

Status encode_byte_to_lsb(unsigned char data, unsigned char *image_buffer) {
    for (int i = 0; i < 8; i++) {
        image_buffer[i] &= ~1;
        image_buffer[i] |= data >> (7 - i);
    }
    return e_success;
}

Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image,
                            FILE *fptr_stego_image) {
    LOG_INFO(INFO_ENCODING_START);
    unsigned char image_buffer[8];

    for (int i = 0; i < size; i++) {

        if (fread(image_buffer, sizeof(image_buffer), 1, fptr_src_image) != 1) {
            return e_failure;
        }

        encode_byte_to_lsb(data[i], image_buffer);

        if (fwrite(image_buffer, sizeof(image_buffer), 1, fptr_stego_image) !=
            1) {
            return e_failure;
        }
    }

    LOG_INFO(INFO_ENCODING_DONE);
    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo) {
    LOG_INFO(INFO_ENCODE_MAGIC);

    if (encode_data_to_image(magic_string, strlen(magic_string),
                             encInfo->fptr_src_image,
                             encInfo->fptr_stego_image) == e_failure) {
        LOG_ERROR(ERR_ENCODING_FAILED);
        return e_failure;
    }

    return e_success;
}

Status encode_secret_file_extn_size(EncodeInfo *encInfo) {
    LOG_INFO(INFO_ENCODE_EXTENSION_SIZE);
    char extn_size = strlen(encInfo->extn_secret_file);
    if (encode_data_to_image(&extn_size, 1, encInfo->fptr_src_image,
                             encInfo->fptr_stego_image) == e_failure) {
        return e_failure;
    }
    return e_success;
}

Status encode_secret_file_extn(EncodeInfo *encInfo) {
    LOG_INFO(INFO_ENCODE_EXTENSION);
    if (encode_data_to_image(
            encInfo->extn_secret_file, strlen(encInfo->extn_secret_file),
            encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure) {
        return e_failure;
    }
    return e_success;
}

Status encode_secret_file_size(EncodeInfo *encInfo) {
    LOG_INFO(INFO_ENCODE_FILE_SIZE);
    char *sizeptr = (char *)&encInfo->size_secret_file;
    if (encode_data_to_image(sizeptr, 4, encInfo->fptr_src_image,
                             encInfo->fptr_stego_image) == e_failure) {
        return e_failure;
    }
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo) {
    LOG_INFO(INFO_ENCODE_FILE_DATA);
    char *secret_data = malloc(encInfo->size_secret_file);
    fread(secret_data, encInfo->size_secret_file, 1, encInfo->fptr_secret);
    if (encode_data_to_image(secret_data, encInfo->size_secret_file,
                             encInfo->fptr_src_image,
                             encInfo->fptr_stego_image) == e_failure) {
        return e_failure;
    }
    free(secret_data);
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest) {
    LOG_INFO(INFO_COPY_REMAINING);

    long unsigned curr_offset = ftell(fptr_src);
    fseek(fptr_src, 0, SEEK_END);
    long unsigned file_end = ftell(fptr_src);
    fseek(fptr_src, curr_offset, SEEK_SET);
    long unsigned remaining_size = file_end - curr_offset;

    char *remaining_buffer = malloc(remaining_size);

    if (fread(remaining_buffer, remaining_size, 1, fptr_src) != 1)
        return e_failure;
    if (fwrite(remaining_buffer, remaining_size, 1, fptr_dest) != 1)
        return e_failure;

    free(remaining_buffer);

    return e_success;
}

Status close_encode_files(EncodeInfo *encInfo) {
    LOG_INFO(INFO_CLOSE_FILES);
    if (fclose(encInfo->fptr_stego_image) == EOF)
        return e_failure;
    if (fclose(encInfo->fptr_src_image) == EOF)
        return e_failure;
    if (fclose(encInfo->fptr_secret) == EOF)
        return e_failure;
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo) {
    // open files
    if (open_encode_files(encInfo) == e_failure) {
        LOG_ERROR(ERR_OPEN_FILES);
        return e_failure;
    }
    // validate file size - header size > encode data size
    if (check_capacity(encInfo) == e_failure) {
        LOG_ERROR(ERR_INSUFFICIENT_CAPACITY);
        return e_failure;
    }
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) ==
        e_failure) {
        LOG_ERROR(ERR_COPY_HEADER);
        return e_failure;
    }
    // encode magic
    if (encode_magic_string(MAGIC_STRING, encInfo) == e_failure) {
        LOG_ERROR(ERR_ENCODE_MAGIC);
        return e_failure;
    }
    // encode extension size
    if (encode_secret_file_extn_size(encInfo) == e_failure) {
        LOG_ERROR(ERR_ENCODE_EXTENSION_SIZE);
        return e_failure;
    }
    // encode extension name
    if (encode_secret_file_extn(encInfo) == e_failure) {
        LOG_ERROR(ERR_ENCODE_EXTENSION);
        return e_failure;
    }
    // encode secret file size
    if (encode_secret_file_size(encInfo) == e_failure) {
        LOG_ERROR(ERR_ENCODE_FILE_SIZE);
        return e_failure;
    }
    // encode secret file
    if (encode_secret_file_data(encInfo) == e_failure) {
        LOG_ERROR(ERR_ENCODE_FILE_DATA);
        return e_failure;
    }
    // copy remaining data
    if (copy_remaining_img_data(encInfo->fptr_src_image,
                                encInfo->fptr_stego_image) == e_failure) {
        LOG_ERROR(ERR_COPY_REMAINING);
        return e_failure;
    }
    // close all files
    if (close_encode_files(encInfo) == e_failure) {
        LOG_ERROR(ERR_CLOSE_FILES);
        return e_failure;
    }
    return e_success;
}
