//
// Created by Yucheng Soku on 2024/10/23.
//

#ifndef TERRAINTILEBUILDER_KDTREE_H
#define TERRAINTILEBUILDER_KDTREE_H

#include <vector>
#include "nanoflann.hpp"
#include "Utils/Point.h"
#include "Utils/Geo/GeoUtils.h"

namespace TTB {

    struct PointCloud {

        std::vector<Point> points;

        [[nodiscard]] size_t kdtree_get_point_count() const { return points.size(); }

        [[nodiscard]] inline double kdtree_get_pt(size_t index, size_t dim) const {
            if (dim == 0) return points[index].x;
            else if (dim == 1) return points[index].y;
            else return points[index].z;
        }

        inline double kdtree_distance(const double* p1, size_t idx_p2, size_t /*size*/) const {
            Point point1 = {p1[0], p1[1], p1[2]};
            const Point& p2 = points[idx_p2];
            return haversineDistance(point1, p2);
        }

        template<class BBOX>
        bool kdtree_get_bbox(BBOX&) const  { return false; }
    };

    typedef nanoflann::KDTreeSingleIndexAdaptor<
            nanoflann::L2_Simple_Adaptor<double, PointCloud>,
            PointCloud,
            2
    > KdTreePrototype;

    KdTreePrototype buildKdTree(const PointCloud& cloud, int dim, int maxLeaf = 10);
}

#endif //TERRAINTILEBUILDER_KDTREE_H
