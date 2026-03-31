#include "encode.h"
#include "types.h"
#include <stdio.h>
#include <string.h>

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image) {
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

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo) {
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL) {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",
                encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL) {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",
                encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL) {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",
                encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}

OperationType check_operation_type(char *arg) {
    if (strcmp(arg, "-e") == 0)
        return e_encode;
    else if (strcmp(arg, "-d") == 0)
        return e_decode;
    else
        return e_unsupported;
}

Status read_and_validate_encode_args(char **argv, EncodeInfo *encInfo) {
    // check .bmp file

    if (strstr(argv[2], ".bmp") == NULL) {
        printf("%s", argv[2]);
        return e_failure;
    }
    // check . present in secret file

    if (strchr(argv[3], '.') == NULL)
        return e_failure;
    // check if output file is present
    // if not default

    if (argv[4] && strstr(argv[4], ".bmp") == NULL)
        return e_failure;
    // else validate it to be .bmp file as well

    encInfo->src_image_fname = argv[2];
    encInfo->secret_fname = argv[3];

    if (argv[4])
        encInfo->stego_image_fname = argv[4];
    else
        encInfo->stego_image_fname = "4-SkeletonCode/stego_img.bmp";

    return e_success;
}

Status check_capacity(EncodeInfo *encInfo) {
    // compare rgb data size to encoded text size
    uint secret_size = 0;
    uint file_size = get_image_size_for_bmp(encInfo->fptr_src_image);

    // encoded text size =
    // magic string size + 1 unsigned char to represent magic string

    char *magicString = "#*";
    secret_size += strlen(magicString) + 1;
    // extension size + 1 unsigned char to represent extension size

    char *extension =
        &encInfo->secret_fname[strcspn(encInfo->secret_fname, ".")];

    uint extensionLen = strlen(extension);
    secret_size += extensionLen + 1;
    // file size + 1 int (4 bytes) to represent file size

    secret_size += encInfo->size_secret_file + 4;

    if (file_size < secret_size * 8)
        return e_failure;
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo) {
    // open files
    if (open_files(encInfo) == e_failure)
        return e_failure;
    // validate file size - header size > encode data size
    if (check_capacity(encInfo) == e_failure)
        return e_failure;
    // encode magic
    // encode extension size
    // encode extension name
    // encode secret file size
    // encode secret file
    // copy remaining data
    return e_success;
}
