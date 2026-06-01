cmake_minimum_required(VERSION 3.16)

project(ImgConverter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(../ImgLib ImgLibBuildDir)

add_executable(imgconv main.cpp)

target_include_directories(imgconv PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../ImgLib")
target_link_libraries(imgconv PRIVATE ImgLib)

target_compile_options(imgconv PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
