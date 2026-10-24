#ifndef POLYGON_H
#define POLYGON_H

#include "Geometry.h"
#include <vector>
#include <stdexcept>

class Polygon {
private:
    std::vector<Point> vertices;
    bool valid;

    bool isSimplePolygon() const {
        int n = vertices.size();
        if (n < 3) return false;

        for (int i = 0; i < n; i++) {
            for (int j = i + 2; j < n; j++) {
                if (i == 0 && j == n - 1) continue;

                if (GeometryUtils::segmentsIntersect(
                    vertices[i], vertices[(i + 1) % n],
                    vertices[j], vertices[(j + 1) % n])) {
                    return false;
                }
            }
        }
        return true;
    }

    bool hasNonZeroArea() const {
        double area = GeometryUtils::polygonSignedArea(vertices);
        return std::abs(area) > GeometryUtils::EPS;
    }

    bool hasDistinctVertices() const {
        int n = vertices.size();
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (vertices[i] == vertices[j]) return false;
            }
        }
        return true;
    }

public:
    Polygon() : valid(false) {}

    Polygon(const std::vector<Point>& pts) : vertices(pts), valid(false) {
        validate();
    }

    void validate() {
        valid = false;

        if (vertices.size() < 3) {
            std::cerr << "Error: Polygon must have at least 3 vertices\n";
            return;
        }

        if (!hasDistinctVertices()) {
            std::cerr << "Error: Polygon contains duplicate vertices\n";
            return;
        }

        if (!hasNonZeroArea()) {
            std::cerr << "Error: Polygon has zero area\n";
            return;
        }

        if (!isSimplePolygon()) {
            std::cerr << "Error: Polygon is not simple (edges intersect)\n";
            return;
        }

        double area = GeometryUtils::polygonSignedArea(vertices);
        if (area < 0) {
            std::reverse(vertices.begin(), vertices.end());
        }

        valid = true;
    }

    bool isValid() const { return valid; }

    const std::vector<Point>& getVertices() const { return vertices; }

    int getVertexCount() const { return vertices.size(); }

    Point getVertex(int i) const { return vertices[i]; }

    int getTriangleCount() const {
        return std::max(0, (int)vertices.size() - 2);
    }

    void printInfo() const {
        std::cout << "Polygon: " << vertices.size() << " vertices\n";
        std::cout << "Valid: " << (valid ? "Yes" : "No") << "\n";
        std::cout << "Area: " << std::abs(GeometryUtils::polygonSignedArea(vertices)) << "\n";
    }
};

#endif
