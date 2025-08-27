#include "bmp_image.h"
#include "pack_defines.h"

#include <array>
#include <fstream>
#include <string_view>
#include <cstdint>
#include <vector>

using namespace std;

namespace img_lib {

PACKED_STRUCT_BEGIN BitmapFileHeader {
    uint16_t signature;
    uint32_t file_size;
    uint32_t reserved;
    uint32_t data_offset;
}
PACKED_STRUCT_END

PACKED_STRUCT_BEGIN BitmapInfoHeader {
    uint32_t header_size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t data_size;
    int32_t h_resolution;
    int32_t v_resolution;
    uint32_t colors_used;
    uint32_t important_colors;
}
PACKED_STRUCT_END

static int GetBMPStride(int w) {
    return 4 * ((w * 3 + 3) / 4);
}

bool SaveBMP(const Path& file, const Image& image) {
    ofstream out(file, ios::binary);
    if (!out) {
        return false;
    }

    const int width = image.GetWidth();
    const int height = image.GetHeight();
    const int stride = GetBMPStride(width);
    const int data_size = stride * height;
    const int file_size = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + data_size;

    BitmapFileHeader file_header;
    file_header.signature = 0x4D42;
    file_header.file_size = file_size;
    file_header.reserved = 0;
    file_header.data_offset = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);
    out.write(reinterpret_cast<const char*>(&file_header), sizeof(file_header));

    BitmapInfoHeader info_header;
    info_header.header_size = sizeof(BitmapInfoHeader);
    info_header.width = width;
    info_header.height = height;
    info_header.planes = 1;
    info_header.bits_per_pixel = 24;
    info_header.compression = 0;
    info_header.data_size = data_size;
    info_header.h_resolution = 11811;
    info_header.v_resolution = 11811;
    info_header.colors_used = 0;
    info_header.important_colors = 0x1000000;
    out.write(reinterpret_cast<const char*>(&info_header), sizeof(info_header));

    vector<char> buffer(stride);
    for (int y = height - 1; y >= 0; --y) {
        const Color* line = image.GetLine(y);
        for (int x = 0; x < width; ++x) {
            buffer[x * 3 + 0] = static_cast<char>(line[x].b);
            buffer[x * 3 + 1] = static_cast<char>(line[x].g);
            buffer[x * 3 + 2] = static_cast<char>(line[x].r);
        }
        for (int x = width * 3; x < stride; ++x) {
            buffer[x] = 0;
        }
        out.write(buffer.data(), stride);
    }

    return out.good();
}

Image LoadBMP(const Path& file) {
    ifstream in(file, ios::binary);
    if (!in) {
        return {};
    }

    BitmapFileHeader file_header;
    in.read(reinterpret_cast<char*>(&file_header), sizeof(file_header));
    if (!in || file_header.signature != 0x4D42) {
        return {};
    }

    BitmapInfoHeader info_header;
    in.read(reinterpret_cast<char*>(&info_header), sizeof(info_header));
    if (!in || info_header.header_size != sizeof(BitmapInfoHeader) ||
        info_header.planes != 1 || info_header.bits_per_pixel != 24 ||
        info_header.compression != 0) {
        return {};
    }

    const int width = info_header.width;
    const int height = info_header.height;
    const int stride = GetBMPStride(width);

    if (width <= 0 || height <= 0) {
        return {};
    }

    Image result(width, height, Color::Black());
    vector<char> buffer(stride);

    for (int y = height - 1; y >= 0; --y) {
        in.read(buffer.data(), stride);
        if (!in) {
            return {};
        }

        Color* line = result.GetLine(y);
        for (int x = 0; x < width; ++x) {
            line[x].b = static_cast<byte>(buffer[x * 3 + 0]);
            line[x].g = static_cast<byte>(buffer[x * 3 + 1]);
            line[x].r = static_cast<byte>(buffer[x * 3 + 2]);
            line[x].a = static_cast<byte>(255);
        }
    }

    return result;
}

}  // namespace img_lib
