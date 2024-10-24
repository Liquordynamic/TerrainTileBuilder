//
// Created by Yucheng Soku on 2024/10/21.
//

#ifndef TERRAINTILEBUILDER_CIDW_H
#define TERRAINTILEBUILDER_CIDW_H

#include <filesystem>
#include "Reflection/Reflector.h"
#include "TerrainTileBuilder/ICore.h"

namespace fs = std::filesystem;

namespace TTB {

    class CIDW : public ICore {

    private:
        void m_initResourceDirectoryStructure() override;

    public:

        const char* getDisplayName() override { return "CPU IDW COMPUTATION CORE"; }

        void execute(
                std::vector<Point>&     samplePoints,
                std::array<double, 4>&  extent
            ) override;
    };

    REGISTER_CLASS(CIDW)
}

#endif //TERRAINTILEBUILDER_CIDW_H
