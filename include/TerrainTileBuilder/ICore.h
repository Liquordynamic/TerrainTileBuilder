//
// Created by Yucheng Soku on 2024/10/21.
//

#ifndef TERRAINTILEBUILDER_ICORE_H
#define TERRAINTILEBUILDER_ICORE_H

#include "Utils/Point.h"
#include "Reflection/IObject.h"

namespace fs = std::filesystem;

namespace TTB {

    class ICore : public IObject {

    public:
        fs::path outputPath;
        int tileSize;
        int fromZoom;
        int toZoom;

        ICore() {
            outputPath = "";
            tileSize = 0;
            fromZoom = 0;
            toZoom = 0;
        }

        virtual const char* className() { return "ICore"; }

        virtual void initialize(
                const char*             _outputPath,
                int                     _tileSize,
                int                     _fromZoom,
                int                     _toZoom
        ) {
            outputPath = _outputPath;
            tileSize = _tileSize;
            fromZoom = _fromZoom;
            toZoom = _toZoom;
        };

        virtual void execute(
                std::vector<Point>&     samplePoints,
                std::array<double, 4>&  extent
        ) = 0;
    };

}

#endif //TERRAINTILEBUILDER_ICORE_H
