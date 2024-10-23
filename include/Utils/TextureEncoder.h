//
// Created by Yucheng Soku on 2024/10/22.
//

#ifndef TERRAINTILEBUILDER_TEXTUREENCODER_H
#define TERRAINTILEBUILDER_TEXTUREENCODER_H

#include <vector>
#include "Utils/Point.h"

namespace TTB {

    union Texel {
        float f32;
        uint8_t rgba8[4];
    };

    template<typename T, int Channels>
    class TextureEncoder {

    private:
        int                         m_channels;
        int                         m_height;
        int                         m_width;
        std::vector<T>              m_data;

    public:
        TextureEncoder(int width, int height);

        std::vector<T>& getData();

        [[nodiscard]] int getWidth() const;

        [[nodiscard]] int getHeight() const;

        [[nodiscard]] int getChannels() const;

        void dumpToDisk(const char* path);

    };
}
#endif //TERRAINTILEBUILDER_TEXTUREENCODER_H
