#include "encode.h"
#include "logs.h"
#include "types.h"

int main(int argc, char *argv[]) {
    EncodeInfo encInfo;
    // uint img_size;

    if (argc < 3) {
        LOG_ERROR("Add valid options use -h to know more about the options");
        return 0;
    }
    OperationType ret = check_operation_type(argv[1]);
    if (ret == e_unsupported) {
        LOG_ERROR(ERR_CHECK_OPERATION);
        return 0;
    }
    if (ret == e_encode) {
        if (argc < 4 || argc > 5 ||
            read_and_validate_encode_args(argv, &encInfo) == e_failure) {
            LOG_ERROR(ERR_CHECK_ARGUMENTS);
            return 0;
        }
        if (do_encoding(&encInfo) == e_failure) {
            LOG_ERROR(ERR_ENCODING_FAILED);
            return 0;
        }
    } else {
        if (argc < 3 || argc > 4) {
            LOG_ERROR(ERR_ENCODING_FAILED);
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
