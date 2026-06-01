cmake_minimum_required(VERSION 3.16)

project(ImgLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(IMGLIB_MAIN_FILES img_lib.h img_lib.cpp)

set(IMGLIB_FORMAT_FILES
    ppm_image.h ppm_image.cpp
    jpeg_image.h jpeg_image.cpp
    bmp_image.h bmp_image.cpp
)

add_library(ImgLib STATIC
    ${IMGLIB_MAIN_FILES}
    ${IMGLIB_FORMAT_FILES}
)

target_include_directories(ImgLib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

if(LIBJPEG_DIR)
    message(STATUS "Using LibJPEG from ${LIBJPEG_DIR}")
    target_include_directories(ImgLib PUBLIC "${LIBJPEG_DIR}/include")
    target_link_directories(ImgLib PUBLIC
        "${LIBJPEG_DIR}/lib/$<IF:$<CONFIG:Debug>,Debug,Release>"
    )
    target_link_libraries(ImgLib PUBLIC jpeg)
else()
    find_package(JPEG REQUIRED)
    target_link_libraries(ImgLib PUBLIC JPEG::JPEG)
endif()

target_compile_options(ImgLib PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
