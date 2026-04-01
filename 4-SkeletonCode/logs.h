#ifndef LOG_H
#define LOG_H

/* =======================
   INFO LOGS
   ======================= */

#define INFO_CHECK_OPERATION "Checking if operation is valid"
#define INFO_CHECK_ARGUMENTS "Checking if arguments are valid"
#define INFO_OPEN_FILES "Opening required files"
#define INFO_CLOSE_FILES "Closing files"
#define INFO_OPENED_BMP "Opened input BMP file"
#define INFO_OPENED_SECRET "Opened secret file"
#define INFO_OPENED_OUTPUT "Opened output file"
#define INFO_DONE "Done"
#define INFO_ENCODING_START "## Encoding Procedure Started ##"
#define INFO_CHECK_SECRET_SIZE "Checking secret file size"
#define INFO_CHECK_RGB_DATA_SIZE "Checking rgb data size"
#define INFO_SECRET_NOT_EMPTY "Done. Not Empty"
#define INFO_CHECK_CAPACITY "Checking BMP capacity for secret data"
#define INFO_CAPACITY_OK "Done. Found OK"
#define INFO_DEFAULT_OUTPUT                                                    \
    "Output file not mentioned. Creating default output file"
#define INFO_COPY_HEADER "Copying Image Header"
#define INFO_ENCODE_MAGIC "Encoding Magic String Signature"
#define INFO_ENCODE_EXTENSION "Encoding file extension"
#define INFO_ENCODE_EXTENSION_SIZE "Encoding file extension size"
#define INFO_ENCODE_FILE_SIZE "Encoding file size"
#define INFO_ENCODE_FILE_DATA "Encoding file data"
#define INFO_COPY_REMAINING "Copying Left Over Data"
#define INFO_ENCODING_DONE "## Encoding Done Successfully ##"

/* =======================
   ERROR LOGS
   ======================= */

#define ERR_CHECK_OPERATION "Invalid operation type"
#define ERR_CHECK_ARGUMENTS "Invalid arguments"
#define ERR_OPEN_FILES "Failed to open required files"
#define ERR_CLOSE_FILES "Failed to close files"
#define ERR_OPEN_BMP "Failed to open input BMP file"
#define ERR_OPEN_SECRET "Failed to open secret file"
#define ERR_OPEN_OUTPUT "Failed to create/open output file"
#define ERR_INVALID_BMP "Invalid source file"
#define ERR_INVALID_SECRET "Invalid secret file"
#define ERR_INVALID_OUTPUT "Invalid output file"
#define ERR_GENERIC "Operation failed"
#define ERR_ENCODING_START "Failed to start encoding procedure"
#define ERR_SECRET_EMPTY "Secret file is empty"
#define ERR_CAPACITY_CHECK "Failed to check BMP capacity"
#define ERR_INSUFFICIENT_CAPACITY "Insufficient capacity in BMP file"
#define ERR_DEFAULT_OUTPUT "Failed to create default output file"
#define ERR_COPY_HEADER "Failed to copy BMP header"
#define ERR_ENCODE_MAGIC "Failed to encode magic string"
#define ERR_ENCODE_EXTENSION "Failed to encode file extension"
#define ERR_ENCODE_EXTENSION_SIZE "Failed to encode file extension size"
#define ERR_ENCODE_FILE_SIZE "Failed to encode file size"
#define ERR_ENCODE_FILE_DATA "Failed to encode file data"
#define ERR_COPY_REMAINING "Failed to copy remaining data"
#define ERR_ENCODING_FAILED "Encoding failed"

/* =======================
   OPTIONAL LOG HELPERS
   ======================= */

#define LOG_INFO(msg) printf("INFO: %s\n", msg)
#define LOG_ERROR(msg) fprintf(stderr, "ERROR: %s\n", msg)

#endif
