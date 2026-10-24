#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include "Polygon.h"
#include "Triangle.h"
#include <vector>
#include <list>

class Triangulation {
private:
    std::vector<Triangle> triangles;
    const Polygon& polygon;
    std::vector<Point> vertices;

public:
    Triangulation(const Polygon& poly) : polygon(poly), vertices(poly.getVertices()) {}

    bool triangulate() {
        triangles.clear();

        if (!polygon.isValid()) {
            std::cerr << "Cannot triangulate invalid polygon\n";
            return false;
        }

        int n = vertices.size();
        if (n < 3) return false;

        std::list<int> active;
        for (int i = 0; i < n; i++) {
            active.push_back(i);
        }

        while (active.size() > 3) {
            bool foundEar = false;

            for (auto it = active.begin(); it != active.end(); ++it) {
                auto prev = it;
                auto next = it;
                ++next;

                if (next == active.end()) next = active.begin();
                if (prev == active.begin()) prev = active.end();
                --prev;

                int a = *prev;
                int b = *it;
                int c = *next;

                if (!isConvex(a, b, c)) continue;

                if (hasInteriorVertex(a, b, c, active, it)) continue;

                triangles.emplace_back(a, b, c);
                it = active.erase(it);
                if (it != active.begin()) --it;

                foundEar = true;
                break;
            }

            if (!foundEar) {
                std::cerr << "Failed to find ear during triangulation\n";
                triangles.clear();
                return false;
            }
        }

        auto it = active.begin();
        int v0 = *it;
        ++it;
        int v1 = *it;
        ++it;
        int v2 = *it;

        triangles.emplace_back(v0, v1, v2);

        int expectedCount = polygon.getVertexCount() - 2;
        if ((int)triangles.size() != expectedCount) {
            std::cerr << "Triangulation produced " << triangles.size()
                      << " triangles, expected " << expectedCount << "\n";
            triangles.clear();
            return false;
        }

        return true;
    }

    const std::vector<Triangle>& getTriangles() const {
        return triangles;
    }

    int getTriangleCount() const {
        return triangles.size();
    }

    bool isValid() const {
        return !triangles.empty() && (int)triangles.size() == polygon.getTriangleCount();
    }

private:
    bool isConvex(int a, int b, int c) const {
        return GeometryUtils::isConvex(vertices[a], vertices[b], vertices[c]);
    }

    bool hasInteriorVertex(int a, int b, int c, const std::list<int>& active,
                           std::list<int>::iterator excludeIt) const {
        for (auto it = active.begin(); it != active.end(); ++it) {
            if (it == excludeIt) continue;
            if (*it == a || *it == b || *it == c) continue;

            if (GeometryUtils::pointInTriangle(vertices[*it], vertices[a], vertices[b], vertices[c])) {
                return true;
            }
        }
        return false;
    }
};

#endif
