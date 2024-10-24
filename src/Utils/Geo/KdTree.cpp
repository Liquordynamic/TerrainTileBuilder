//
// Created by Yucheng Soku on 2024/10/23.
//

#include "Utils/KdTree.h"

namespace TTB {

    KdTreePrototype buildKdTree(PointCloud& cloud, int dim, int maxLeaf) {
        return KdTreePrototype(dim, cloud, nanoflann::KDTreeSingleIndexAdaptorParams(maxLeaf));
    }
}