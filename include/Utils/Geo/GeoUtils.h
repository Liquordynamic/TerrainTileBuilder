//
// Created by Yucheng Soku on 2024/10/21.
//

#ifndef TERRAINTILEBUILDER_GEOUTILS_H
#define TERRAINTILEBUILDER_GEOUTILS_H

#include <array>

namespace TTB {

    template<typename T>
    struct Tile {
        T x;
        T y;
        T z;
    };

    constexpr double EARTH_RADIUS = 6371000.0;

    double haversineDistance(const Point& geoPoint1, const Point& geoPoint2);

    double tileToLon(int x, int z);

    double tileToLat(int y, int z);

    void tileToBBox(Tile<int>& tile, std::array<double, 4>& bbox);

    double mercatorXFromLon(double lon);

    double mercatorYFromLat(double lat);

    std::array<double, 2> fromLonLat(double lon, double lat);

    double lonFromMercatorX(double x);

    double latFromMercatorY(double y);

    std::array<double, 2> fromXY(double x, double y);

    Tile<double> pointToTileFraction(double lon, double lat, double z);

    Tile<int> pointToTile(double lon, double lat, double z);
}

#endif //TERRAINTILEBUILDER_GEOUTILS_H
