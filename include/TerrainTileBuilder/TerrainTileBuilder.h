//
// Created by Yucheng Soku on 2024/10/21.
//

#ifndef TERRAINTILEBUILDER_TERRAINTILEBUILDER_H
#define TERRAINTILEBUILDER_TERRAINTILEBUILDER_H

#include <array>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;
namespace TTB {

    class TerrainTileBuilder {

    void m_createResourceDirectory() const;

    public:
        int                                 toZoom;
        int                                 fromZoom;
        int                                 tileSize;
        fs::path                            outputPath;

        TerrainTileBuilder(
                int                         _tileSize,
                int                         _fromZoom,
                int                         _toZoom,
                const char*                 _outputPath
            );

        void build(
                const char*                 coreName,
                std::vector<Point>&         samplePoints,
                std::array<double, 4>&      extent
            ) const;
    };

}

#endif //TERRAINTILEBUILDER_TERRAINTILEBUILDER_H
