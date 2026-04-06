cmake_minimum_required(VERSION 3.10)

project(LSBSteganography
    VERSION 1.0
    DESCRIPTION "LSB Steganography of any files"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(steg
    src/main.c
    src/encode.c
    src/decode.c
)

target_compile_options(steg
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)
