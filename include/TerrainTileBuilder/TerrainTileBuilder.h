//
// Created by Yucheng Soku on 2024/10/21.
//

#ifndef TERRAINTILEBUILDER_TERRAINTILEBUILDER_H
#define TERRAINTILEBUILDER_TERRAINTILEBUILDER_H

#include <array>
#include <vector>
#include <string>
#include <filesystem>
#include "TerrainTileBuilder/ICore.h"

namespace fs = std::filesystem;

namespace TTB {

    class TerrainTileBuilder {

    private:
        void m_createResourceDirectory() const;

    public:
        int toZoom;
        int fromZoom;
        int tileSize;
        fs::path outputPath;
        const char* coreName;

        TerrainTileBuilder(
                const char* coreName,
                int _tileSize,
                int _fromZoom,
                int _toZoom,
                const char* _outputPath
            );

        void build(std::vector<Point>& samplePoints, std::array<double, 4>& extent) const;
    };

}

#endif //TERRAINTILEBUILDER_TERRAINTILEBUILDER_H
