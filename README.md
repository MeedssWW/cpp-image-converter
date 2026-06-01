# C++ Image Converter

[![C++ CI](https://github.com/MeedssWW/cpp-image-converter/actions/workflows/cpp-ci.yml/badge.svg)](https://github.com/MeedssWW/cpp-image-converter/actions/workflows/cpp-ci.yml)

Image Converter is a command-line tool for converting images between supported formats. The project contains a small image library and a CLI application that detects formats by file extension, loads the input image, and saves it in the requested output format.

## Supported Formats

- JPEG / JPG
- BMP
- PPM

## Features

- Converts images using a simple CLI interface.
- Detects input and output formats by extension.
- Uses a shared internal `Image` representation.
- Contains separate modules for JPEG, BMP, and PPM loading/saving.
- Uses libjpeg for JPEG encoding and decoding.

## Tech Stack

- C++17 / C++20
- STL
- CMake
- libjpeg
- Binary file I/O
- Basic image format parsing

## Project Structure

```text
.
|-- ImgConverter/
|   |-- CMakeLists.txt
|   `-- main.cpp              # CLI application
`-- ImgLib/
    |-- CMakeLists.txt
    |-- img_lib.*             # Shared image model
    |-- jpeg_image.*          # JPEG load/save
    |-- bmp_image.*           # BMP load/save
    |-- ppm_image.*           # PPM load/save
    `-- pack_defines.h        # Packed struct helpers for BMP
```

## Build

The project is split into `ImgLib` and `ImgConverter` CMake targets. A typical build flow is:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

If your environment does not have a root `CMakeLists.txt`, create one that adds both subdirectories:

```cmake
cmake_minimum_required(VERSION 3.11)
project(cpp_image_converter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(ImgLib)
add_subdirectory(ImgConverter)
```

## Usage

```bash
img_converter <input_file> <output_file>
```

Examples:

```bash
img_converter photo.jpg photo.bmp
img_converter image.ppm image.jpg
img_converter input.bmp output.ppm
```

## What This Project Demonstrates

- Designing a small reusable image library.
- Working with binary file formats.
- Integrating a C library from C++ code.
- Separating format-specific logic from the CLI layer.
- Using polymorphism to handle multiple image formats through one interface.

## Possible Improvements

- Add a root `CMakeLists.txt`.
- Add sample images and conversion examples.
- Add automated tests for PPM/BMP round trips.
- Add CI build checks on Linux.
- Improve error messages for unsupported or corrupted files.
