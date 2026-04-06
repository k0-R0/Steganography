#include "decode.h"
#include "encode.h"
#include "logs.h"
#include "types.h"
#include <string.h>

/* Check operation type */
OperationType check_operation_type(char *arg) {
    LOG_INFO(INFO_CHECK_OPERATION);
    if (strcmp(arg, "-e") == 0)
        return e_encode;
    else if (strcmp(arg, "-d") == 0)
        return e_decode;
    else
        return e_unsupported;
}

int main(int argc, char *argv[]) {

    if (argc < 3) {
        LOG_ERROR(ERR_CHECK_ARGUMENTS);
        PRINT_ENCODE_HELP();
        PRINT_DECODE_HELP();
        return 0;
    }
    OperationType ret = check_operation_type(argv[1]);

    if (ret == e_unsupported) {

        LOG_ERROR(ERR_CHECK_OPERATION);
        PRINT_ENCODE_HELP();
        PRINT_DECODE_HELP();

        return 0;

    } else if (ret == e_encode) {

        EncodeInfo encInfo;
        if (argc < 4 || argc > 5 ||
            read_and_validate_encode_args(argv, &encInfo) == e_failure) {

            LOG_ERROR(ERR_CHECK_ARGUMENTS);
            PRINT_ENCODE_HELP();

            return 0;
        }
        if (do_encoding(&encInfo) == e_failure) {
            LOG_ERROR(ERR_ENCODING_FAILED);

            return 0;
        }
    } else {

        DecodeInfo decInfo;
        if (argc < 3 || argc > 4 ||
            read_and_validate_decode_args(argv, &decInfo) == e_failure) {

            LOG_ERROR(ERR_CHECK_ARGUMENTS);
            PRINT_DECODE_HELP();

            return 0;
        }
        if (do_decoding(&decInfo) == e_failure) {
            LOG_ERROR(ERR_ENCODING_FAILED);

            return 0;
        }
    }
}
