//
// Created by Yucheng Soku on 2024/10/21.
//

#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

#include "Utils/Point.h"
#include "Config.h"
#include "Reflection/Reflector.h"
#include "Utils/TextureEncoder.h"
#include "TerrainTileBuilder/ICore.h"
#include "TerrainTileBuilder/TerrainTileBuilder.h"

namespace fs = std::filesystem;

namespace TTB {

    TerrainTileBuilder::TerrainTileBuilder(
            const char*             _coreName,
            int                     _tileSize,
            int                     _fromZoom,
            int                     _toZoom,
            const char*             _outputPath
    ):
            toZoom                  {_toZoom},
            fromZoom                {_fromZoom},
            coreName                {_coreName},
            tileSize                {_tileSize}
    {
        outputPath = RESOURCE_DIR / fs::path(_outputPath);
        m_createResourceDirectory();
    }

    void TerrainTileBuilder::m_createResourceDirectory() const {

        if (!fs::exists(outputPath)) {
            fs::create_directory(outputPath);
        }
        for (int level = fromZoom; level <= toZoom; ++level) {

            auto levelPath = fs::path(outputPath) / std::to_string(level);
            if (!fs::exists(levelPath)) {
                fs::create_directory(levelPath);
            }
        }
    }

    void TerrainTileBuilder::build(std::vector<Point>& samplePoints, std::array<double, 4>& extent) const {

        auto coreObject = Reflector::instance().create(coreName);
        auto core = dynamic_cast<ICore*>(coreObject.get());

        if (core) {
            std::cout << "========= Using Generation Core < " << core->className() << " > =========" << std::endl;
        } else {
            std::cout << "Unknown object type: " << coreName << std::endl;
            return;
        }

        core->initialize(outputPath.c_str(), tileSize, fromZoom, toZoom);
        core->execute(samplePoints, extent);
    }

}
