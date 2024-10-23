//
// Created by Yucheng Soku on 2024/10/22.
//

#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "Utils/TextureEncoder.h"

namespace TTB {

    template<typename T, int Channels>
    TextureEncoder<T, Channels>::TextureEncoder(int width, int height)
            : m_width(width), m_height(height), m_channels(Channels), m_data(width * height * Channels)
    {}

    template<typename T, int Channels>
    std::vector<T>& TextureEncoder<T, Channels>::getData() {
        return m_data;
    }

    template<typename T, int Channels>
    int TextureEncoder<T, Channels>::getWidth() const {
        return m_width;
    }

    template<typename T, int Channels>
    int TextureEncoder<T, Channels>::getHeight() const {
        return m_height;
    }

    template<typename T, int Channels>
    int TextureEncoder<T, Channels>::getChannels() const {
        return m_channels;
    }

    template<typename T, int Channels>
    void TextureEncoder<T, Channels>::dumpToDisk(const char *path) {
        stbi_write_png(path, m_width, m_height, m_channels, m_data.data(), 0);
    }

    template class TextureEncoder<uint8_t, 3>;
}
