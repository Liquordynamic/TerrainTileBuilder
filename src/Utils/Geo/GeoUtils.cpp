#include <cmath>

#include "Utils/Point.h"
#include "Config.h"
#include "Utils/Geo/GeoUtils.h"

namespace TTB {

    double tileToLon(int x, int z) {

        return (double(x) / (1 << z)) * 360.0 - 180.0;
    }

    double tileToLat(int y, int z) {

        auto n = PI - 2.0 * PI * double(y) / (1 << z);
        return RAD_TO_DEG * atan(sinh(n));
    }

    void tileToBBox(Tile<int>& tile, std::array<double, 4>& bbox) {

        bbox[0] = tileToLon(tile.x, tile.z);
        bbox[1] = tileToLat(tile.y, tile.z);
        bbox[2] = tileToLon(tile.x + 1, tile.z);
        bbox[3] = tileToLat(tile.y + 1, tile.z);
    }


    double mercatorXFromLon(double lon) {
        return (180.0 + lon) / 360.0;
    }

    double mercatorYFromLat(double lat) {
        return (180.0 - (180.0 / PI * log(tan(PI / 4.0 + lat * PI / 360.0)))) / 360.0;
    }

    std::array<double, 2> fromLonLat(double lon, double lat) {

        auto x = mercatorXFromLon(lon);
        auto y = mercatorYFromLat(lat);

        x = x * 2.0 - 1.0;
        y = 1.0 - y * 2.0;

        return std::array<double, 2>{x, y};
    }

    double lonFromMercatorX(double x) {

        return x * 360.0 - 180.0;
    }

    double latFromMercatorY(double y) {

        auto y2 = 180.0 - y * 360.0;
        return 360.0 / PI * atan(exp(y2 * PI / 180.0)) - 90.0;
    }

    std::array<double, 2> fromXY(double x, double y) {

        auto lon = lonFromMercatorX(x);
        auto lat = latFromMercatorY(y);

        return std::array<double, 2>{lon, lat};
    }

    double haversineDistance(const Point& geoPoint1, const Point& geoPoint2) {

        double lon1 = geoPoint1.x, lon2 = geoPoint2.x;
        double lat1 = geoPoint1.y, lat2 = geoPoint2.y;

        double lon1Rad = lon1 * DEG_TO_RAD;
        double lat1Rad = lat1 * DEG_TO_RAD;
        double lon2Rad = lon2 * DEG_TO_RAD;
        double lat2Rad = lat2 * DEG_TO_RAD;

        double dLat = lat2Rad - lat1Rad;
        double dLon = lon2Rad - lon1Rad;

        double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
                   std::cos(lat1Rad) * std::cos(lat2Rad) *
                   std::sin(dLon / 2) * std::sin(dLon / 2);

        double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

        return EARTH_RADIUS * c;
    }

    Tile<double> pointToTileFraction(double lon, double lat, double z) {

        double _sin = sin(lat * DEG_TO_RAD);
        double z2 = pow(2.0, z);

        double x = z2 * (lon / 360.0 + 0.5);
        double y = z2 * (0.5 - 0.25 * log((1.0 + _sin) / (1.0 - _sin)) / PI);

        x = fmod(x, z2);
        if (x < 0.0) x = x + z2;

        return Tile<double> { x, y, z };
    }

    Tile<int> pointToTile(double lon, double lat, double z) {

        auto tile = pointToTileFraction(lon, lat, z);
        tile.x = floor(tile.x);
        tile.y = floor(tile.y);
        return Tile<int> { int(tile.x), int(tile.y), int(tile.z) };
    }
}