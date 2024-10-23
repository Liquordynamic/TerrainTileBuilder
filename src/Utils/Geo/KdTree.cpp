//
// Created by Yucheng Soku on 2024/10/23.
//

#include "Utils/Point.h"
#include "Utils/KdTree.h"
#include "Utils/Geo/GeoUtils.h"

namespace TTB {

//    [[nodiscard]] size_t PointCloud::kdtree_get_point_count() const { return points.size(); }
//
//    [[nodiscard]] double PointCloud::kdtree_get_pt(const size_t index, const size_t dim) const {
//        if (dim == 0) return points[index].x;
//        else if (dim == 1) return points[index].y;
//        else return points[index].z;
//    }

//    double PointCloud::kdtree_distance(const double* p1, const size_t idx_p2, size_t /*size*/) const {
//        Point point1 = {p1[0], p1[1], p1[2]};
//        const Point& p2 = points[idx_p2];
//        return haversineDistance(point1, p2);
//    }

//    template<class BBOX>
//    bool PointCloud::kdtree_get_bbox(BBOX&) const { return false; }

    KdTreePrototype buildKdTree(PointCloud& cloud, int dim, int maxLeaf) {
        return KdTreePrototype(dim, cloud, nanoflann::KDTreeSingleIndexAdaptorParams(maxLeaf));
    }
}