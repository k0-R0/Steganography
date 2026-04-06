# 🖼️ Steganography Tool (C Project)

A simple C-based steganography tool that allows you to **encode** a secret file into an image and **decode** it back.

---

## 📂 Project Structure
```bash
.
├── Assets
│ ├── beautiful.bmp # Sample cover image
│ └── secret.txt # Sample secret file
├── CMakeLists.txt # Build configuration
├── compile_commands.json # Compilation database (symlink)
├── output.txt # Sample output/logs
├── README.md
├── src
│ ├── common.h # Shared utilities
│ ├── decode.c # Decode implementation
│ ├── decode.h
│ ├── encode.c # Encode implementation
│ ├── encode.h
│ ├── logs.h # Logging macros
│ ├── main.c # Entry point (CLI handling)
│ ├── stego_img.bmp # Sample encoded image
│ └── types.h # Custom type definitions
└── stego_img.bmp # Output image (generated)
```
---

## 🚀 Features

- Encode a secret file into a BMP image
- Decode hidden data from an encoded image
- Input validation and error handling
- Simple CLI interface
- Logging support for debugging

---

## 🧠 How It Works

### 🔐 Encoding
- Takes a **source image** and a **secret file**
- Embeds the secret data into the image (steganography)
- Outputs a **stego image**

### 🔓 Decoding
- Reads the encoded image
- Extracts and reconstructs the hidden data

---

## 🛠️ Components

### `main.c`
- Entry point of the program
- Handles user input and flags
- Calls encode/decode functions accordingly

### `encode.c / encode.h`
- Handles embedding secret data into the image
- Includes all validations and processing logic

### `decode.c / decode.h`
- Extracts hidden data from the encoded image
- Reconstructs the original secret file

### `logs.h`
- Contains logging macros for debugging and tracing execution

### `common.h`
- Shared helper functions/utilities

### `types.h`
- Custom type definitions used across the project

### `Assets/`
- Contains sample files for testing:
  - `beautiful.bmp` → cover image
  - `secret.txt` → file to hide

---

## ⚙️ Build Instructions

```bash
mkdir -p build
cd build
cmake ..
make
```

## Usage

- Encode
```bash
./stego -e <input.bmp> <secret.txt> [output.bmp]
```

- Decode
```bash
./stego -d <encoded.bmp> [output_file]
```

- Demo
```bash
# Encode
./stego -e Assets/beautiful.bmp Assets/secret.txt stego_img.bmp

# Decode
./stego -d stego_img.bmp extracted.txt
```

## 📌 Notes
Only BMP images are supported currently
Ensure the image has enough capacity to store the secret file
Logs can be enabled/modified via logs.h
