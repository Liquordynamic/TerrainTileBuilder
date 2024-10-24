//
// Created by Yucheng Soku on 2024/10/21.
//

#include <vector>
#include <iostream>
#include "Config.h"
#include "Utils/Point.h"
#include "Utils/KdTree.h"
#include "Utils/Geo/GeoUtils.h"
#include "Utils/TextureEncoder.h"
#include "TerrainTileBuilder/CCore/CIDW.h"

namespace fs = std::filesystem;

namespace TTB {

    template<typename T>
    inline T interpolate(T a, T b, double t) {

        return (1.0 - t) * a + t * b;
    }

    double inverseDistanceWeighting(
            const std::vector<Point>&           points,
            const double*                       target,
            KdTreePrototype &                   kd_tree,
            double                              power       = 1.0,
            size_t                              k           = 10
    )
    {

        std::vector<size_t> ret_index(k);
        std::vector<double> out_dist(k);

        nanoflann::KNNResultSet<double> resultSet(k);
        resultSet.init(&ret_index[0], &out_dist[0]);
        kd_tree.findNeighbors(resultSet, target, nanoflann::SearchParameters(10));

        double numerator = 0.0;
        double denominator = 0.0;

        for (size_t i = 0; i < k; ++i) {

            const auto& point = points[ret_index[i]];
            double dist = out_dist[i];
            if (dist == 0.0) {
                return point.z;
            }
            double weight = 1.0 / std::pow(dist, power);
            numerator += weight * point.z;
            denominator += weight;
        }
        return numerator / denominator;
    }

    void getTilesInExtent(std::vector<Tile<int>>& tiles, const std::array<double, 4>& extent, int zoomLevel) {

        auto tileBL = pointToTile(extent[0], extent[1], zoomLevel);
        auto tileTR = pointToTile(extent[2], extent[3], zoomLevel);
        auto minX = std::max(tileBL.x, 0);
        auto minY = std::max(tileTR.y, 0);
        auto maxX = tileTR.x;
        auto maxY = tileBL.y;

        tiles.clear();
        tiles.reserve((maxX - minX + 1) * (maxY - minY + 1));

        for (int x = minX; x <= maxX; ++x)
            for (int y = minY; y <= maxY; ++y)
                tiles.emplace_back(Tile<int> {x, y, zoomLevel});
    }

    void initializeGrid(std::vector<Point>& grid, std::array<double, 4>& extent, int tileSize) {

        auto dTileSize = double(tileSize);
        auto invTileSize = 1.0 / dTileSize;

        double u, v;
        size_t index = 0;
        for (int row = 0; row < tileSize; ++row) {
            v = (double(row) + 0.5) * invTileSize;
            for (int col = 0; col < tileSize; ++col) {
                u = (double(col) + 0.5) * invTileSize;
                grid[index].x = interpolate<double>(extent[0], extent[2], u);
                grid[index].y = interpolate<double>(extent[1], extent[3], v);
                grid[index].z = 0.0;

                ++index;
            }
        }
    }

    void CIDW::m_initResourceDirectoryStructure() {

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

    void CIDW::execute(
            std::vector<Point>& samplePoints,
            std::array<double, 4>& extent
    )
    {
        // Build KD-tree
        auto cloud = PointCloud{ .points = samplePoints };
        auto tree = buildKdTree(cloud, 2);
        tree.buildIndex();

        // Initialize shared variables
        auto tiles = std::vector<Tile<int>>{};
        auto bbox = std::array<double, 4>{ 0.0, 0.0, 0.0, 0.0};

        // Initialize texture encoder
        auto encoder = std::make_unique<TextureEncoder<uint8_t, 3>>(tileSize, tileSize);
        auto pixelCount = encoder->getWidth() * encoder->getHeight();
        auto& texture = encoder->getData();
        auto& uvs = encoder->getUVs();

        // Set parallel threads
        #ifdef CPU_PARALLEL_COMPUTATION
        omp_set_num_threads(MAX_NUM_THREADS);
        std::cout << "\n>>> Building Tiles in Parallel Using " << MAX_NUM_THREADS << " Threads\n" << std::endl;
        #endif

        // Generate all tiles
        for (int zoom = fromZoom; zoom <= toZoom; ++zoom) {
            getTilesInExtent(tiles, extent, zoom);

            for (auto& tile : tiles) {
                tileToBBox(tile, bbox);

                #ifdef CPU_PARALLEL_COMPUTATION
                #pragma omp parallel for default(none) shared(pixelCount, uvs, bbox, samplePoints, tree, texture)
                #endif // CPU_PARALLEL_COMPUTATION

                for (int i = 0; i < pixelCount; ++i) {

                    auto lon = interpolate<double>(bbox[0], bbox[2], uvs[i * 2 + 0]);
                    auto lat = interpolate<double>(bbox[1], bbox[3], uvs[i * 2 + 1]);

                    std::array<double, 2> point = { lon, lat };

                    // Terrain-RGB encoding rule: height = -10000 + ((R * 256 * 256 + G * 256 + B) * 0.1)
                    auto height = inverseDistanceWeighting(samplePoints, point.data(), tree);
                    auto RGB = static_cast<uint32_t>((height + 10000.0) * 10);
                    texture[i * 3 + 0] = (RGB >> 16) & 0xFF;
                    texture[i * 3 + 1] = (RGB >> 8) & 0xFF;
                    texture[i * 3 + 2] = RGB & 0xFF;
                }

                auto fileName = std::to_string(tile.x) + "-" + std::to_string(tile.y) + "-" + std::to_string(tile.z) + ".png";
                auto path = outputPath / std::to_string(tile.z) / fileName;
                encoder->dumpToDisk(path.c_str());
            }
        }
    }
}
