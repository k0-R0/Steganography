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
    return e_failure;
}

Status do_decoding(DecodeInfo *decInfo) { return e_failure; }
