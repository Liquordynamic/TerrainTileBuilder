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
    {
        double u, v;
        size_t index = 0;
        auto dWidth = double(width);
        auto dHeight = double(height);
        auto dInvWidth = 1.0 / dWidth;
        auto dInvHeight = 1.0 / dHeight;
        m_uvs = std::vector<double>();
        m_uvs.reserve(width * height * 2);

        for (int row = 0; row < height; ++row) {
            v = (double(row) + 0.5) * dInvHeight;
            for (int col = 0; col < width; ++col) {
                u = (double(col) + 0.5) * dInvWidth;
                m_uvs[index++] = u;
                m_uvs[index++] = v;
            }
        }
    }

    template<typename T, int Channels>
    std::vector<T>& TextureEncoder<T, Channels>::getData() {
        return m_data;
    }

    template<typename T, int Channels>
    const std::vector<double> &TextureEncoder<T, Channels>::getUVs() {
        return m_uvs;
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
