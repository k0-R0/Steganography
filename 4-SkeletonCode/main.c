#include "encode.h"
#include "types.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    EncodeInfo encInfo;
    // uint img_size;

    if (argc < 2) {
        fprintf(
            stderr,
            "ERROR: Add valid options use -h to know more about the options\n");
        return 0;
    }
    OperationType ret = check_operation_type(argv[1]);
    if (ret == e_unsupported) {
        printf("Unsupported Operation\n");
        return 0;
    }
    if (argc < 3) {
        printf("Invalid number of arguments\n");
        return 0;
    }
    if (ret == e_encode) {
        if (argc < 4 ||
            read_and_validate_encode_args(argv, &encInfo) == e_failure) {
            printf("Invalid arguments\n");
            return 0;
        }
        if (do_encoding(&encInfo) == e_failure) {
            printf("Encoding failed\n");
            return 0;
        }
    } else {
        if (argc < 3) {
            printf("Invalid arguments\n");
            return 0;
        }
    }
    // Fill with sample filenames
    // encInfo.src_image_fname = "beautiful.bmp";
    // encInfo.secret_fname = "secret.txt";
    // encInfo.stego_image_fname = "stego_img.bmp";
    //
    // // Test open_files
    // if (open_files(&encInfo) == e_failure) {
    //     printf("ERROR: %s function failed\n", "open_files");
    //     return 1;
    // } else {
    //     printf("SUCCESS: %s function completed\n", "open_files");
    // }
    //
    // // Test get_image_size_for_bmp
    // img_size = get_image_size_for_bmp(encInfo.fptr_src_image);
    // printf("INFO: Image size = %u\n", img_size);
    //
    // return 0;
}
