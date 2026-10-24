#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Geometry.h"
#include <vector>
#include <set>

struct Triangle {
    int v0, v1, v2;

    Triangle() : v0(-1), v1(-1), v2(-1) {}
    Triangle(int a, int b, int c) : v0(a), v1(b), v2(c) {}

    int getVertex(int i) const {
        if (i == 0) return v0;
        if (i == 1) return v1;
        return v2;
    }

    bool contains(int vertexIdx) const {
        return v0 == vertexIdx || v1 == vertexIdx || v2 == vertexIdx;
    }

    bool sharesEdge(const Triangle& other) const {
        int count = 0;
        if (contains(other.v0)) count++;
        if (contains(other.v1)) count++;
        if (contains(other.v2)) count++;
        return count == 2;
    }

    std::vector<int> getVertices() const {
        return {v0, v1, v2};
    }
};

#endif
