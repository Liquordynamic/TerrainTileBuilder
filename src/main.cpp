#include <array>
#include <vector>
#include <chrono>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "Config.h"
#include "Utils/Point.h"
#include "nlohmann/json.hpp"
#include "Utils/PyTrigger.h"
#include "TerrainTileBuilder/TerrainTileBuilder.h"

using TTB::Point;
using std::string;
using std::ifstream;
using Json = nlohmann::json;
namespace fs = std::filesystem;

std::vector<Point> readFile(const char* filePath, std::array<double, 4>& extent) {

    std::vector<Point> samplePoints;

    double minElevation = 999999.0;
    double maxElevation = -999999.0;
    auto readFile =  RESOURCE_DIR + string(filePath);
    ifstream fs(readFile);

    int num;
    fs >> num;
    double x, y, z;

    string nn;
    for (size_t i = 0; i < 3; ++i)
        fs >> nn;

    for (size_t i = 0; i < num; ++i) {

        // Read x, y, z attributes
        for (size_t j = 0; j < 3; ++j) {

            if (j == 0) fs >> x;
            else if (j == 1) fs >> y;
            else fs >> z;
        }

        // Remove sampling points out of the extent
        if (x < extent[0] || x > extent[2] || y < extent[1] || y > extent[3]) continue;
        samplePoints.emplace_back(Point{x, y, z});
        // minElevation = z < minElevation ? z : minElevation;
        // maxElevation = z > maxElevation ? z : maxElevation;
    }

    return samplePoints;
}

int main() {

    // Read description json
    auto descriptionPath = fs::path(RESOURCE_DIR) / "description.json";
    std::ifstream iStream(descriptionPath);
    Json descriptiveJSON;
    iStream >> descriptiveJSON;

    // Parse descriptor
    int toZoom = descriptiveJSON["to_level"];
    int tileSize = descriptiveJSON["tile_size"];
    int fromZoom = descriptiveJSON["from_level"];
    std::string inputFile = descriptiveJSON["input_file"];
    std::string outputPath = descriptiveJSON["output_path"];
    std::array<double, 4> extent = descriptiveJSON["extent"];

    // Preprocess data by python
    // pyTrigger(PYTHON_COORDINATE_CONVERTION, CONDA_ENVIRONMENT);

    // Read data
    auto samplePoints = readFile(inputFile.c_str(), extent);

    // Interpolation
    auto builder = new TTB::TerrainTileBuilder(tileSize, fromZoom, toZoom, outputPath.c_str());

    auto start = std::chrono::high_resolution_clock::now();

    builder->build(TTB::CPU_IDW_CORE, samplePoints, extent);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Build time: " << duration.count() << "ms" << std::endl;

    return 0;
}