#ifndef COMMON_H
#define COMMON_H

#include "types.h"
/* Magic string to identify whether stegged or not */
#define MAGIC_STRING "#*"
#define BMP_HEADER_SIZE 54

/* Check operation type */
OperationType check_operation_type(char *arg);

#endif
