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
    T interpolate(T a, T b, double t) {

        return (1.0 - t) * a + t * b;
    }

    void inverseDistanceWeighting(
            const std::vector<Point>&                                           points,
            Point&                                                              target,
            KdTreePrototype &                                                   kd_tree,
            double                                                              power       = 1.0,
            size_t                                                              k           = 10
    )
    {

        std::vector<size_t> ret_index(k);
        std::vector<double> out_dist(k);

        nanoflann::KNNResultSet<double> resultSet(k);
        resultSet.init(&ret_index[0], &out_dist[0]);
        kd_tree.findNeighbors(resultSet, &target.x, nanoflann::SearchParameters(10));

        double numerator = 0.0;
        double denominator = 0.0;

        for (size_t i = 0; i < k; ++i) {

            const auto& point = points[ret_index[i]];
            double dist = out_dist[i];
            if (dist == 0.0) {
                target.z = point.z;
                return;
            }
            double weight = 1.0 / std::pow(dist, power);
            numerator += weight * point.z;
            denominator += weight;
        }
        target.z = numerator / denominator;
    }

    std::vector<Tile<int>> getTilesInExtent(std::array<double, 4>& extent, int zoomLevel) {

        auto tileBL = pointToTile(extent[0], extent[1], zoomLevel);
        auto tileTR = pointToTile(extent[2], extent[3], zoomLevel);
        auto minX = tileBL.x > 0 ? tileBL.x : 0;
        auto minY = tileTR.y > 0 ? tileTR.y : 0;
        auto maxX = tileTR.x;
        auto maxY = tileBL.y;

        auto tilesInExtext = std::vector<Tile<int>>{};
        for (int x = minX; x <= maxX; ++x)
            for (int y = minY; y <= maxY; ++y)
                tilesInExtext.emplace_back(Tile<int> {x, y, zoomLevel});

        return tilesInExtext;
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

    void CIDW::execute(
            std::vector<Point>& samplePoints,
            std::array<double, 4>& extent
    )
    {
        PointCloud cloud;
        cloud.points = samplePoints;
        auto tree = buildKdTree(cloud, 2);
        tree.buildIndex();

        auto tiles = std::vector<Tile<int>>{};
        auto encoder = std::make_unique<TextureEncoder<uint8_t, 3>>(tileSize, tileSize);
        std::vector<uint8_t>& texture = encoder->getData();
        for (int zoom = fromZoom; zoom <= toZoom; ++zoom) {
            for(auto& tile : getTilesInExtent(extent, zoom)) {
                tiles.emplace_back(tile);
            }
        }
        std::cout << "Tile Count = " << tiles.size() << std::endl;

#ifdef CPU_PARALLEL_COMPUTATION
        std::cout << "Parallel Building Tile in " << omp_get_max_threads() << " threads" << std::endl;
#endif

        for (auto& tile : tiles) {

            auto grid = std::vector<Point>(tileSize * tileSize);
            auto bbox = std::array<double, 4>{ 0.0, 0.0, 0.0, 0.0};
            tileToBBox(tile, bbox);
            initializeGrid(grid, bbox, tileSize);

#ifdef CPU_PARALLEL_COMPUTATION
            omp_set_num_threads(omp_get_max_threads());
            #pragma omp parallel for default(none) shared(grid, samplePoints, std::cout, tree, texture)
#endif // CPU_PARALLEL_COMPUTATION

            for (size_t i = 0; i < grid.size(); ++i) {

                auto point = grid[i];
                inverseDistanceWeighting(samplePoints, point, tree);

                // Terrain-RGB encoding rule: height = -10000 + ((R * 256 * 256 + G * 256 + B) * 0.1)
                auto RGB = static_cast<uint32_t>((point.z + 10000.0) * 10);
                texture[i * 3 + 0] = (RGB >> 16) & 0xFF;
                texture[i * 3 + 1] = (RGB >> 8) & 0xFF;
                texture[i * 3 + 2] = RGB & 0xFF;
            }

            auto textureName = std::to_string(tile.x) + "-" + std::to_string(tile.y) + "-" + std::to_string(tile.z) + ".png";
            auto path = outputPath / std::to_string(tile.z) / textureName;
            encoder->dumpToDisk(path.c_str());
        }
    }
}
