#ifndef COLORING_H
#define COLORING_H

#include "Triangle.h"
#include "Geometry.h"
#include <vector>
#include <queue>
#include <set>

enum Color { RED = 0, GREEN = 1, BLUE = 2, UNCOLORED = -1 };

class Coloring {
private:
    std::vector<Color> vertexColors;
    const std::vector<Point>& vertices;
    const std::vector<Triangle>& triangles;
    std::vector<std::set<int>> adjacencyList;

    void buildAdjacencyGraph() {
        adjacencyList.clear();
        adjacencyList.resize(vertices.size());

        for (const auto& triangle : triangles) {
            adjacencyList[triangle.v0].insert(triangle.v1);
            adjacencyList[triangle.v0].insert(triangle.v2);
            adjacencyList[triangle.v1].insert(triangle.v0);
            adjacencyList[triangle.v1].insert(triangle.v2);
            adjacencyList[triangle.v2].insert(triangle.v0);
            adjacencyList[triangle.v2].insert(triangle.v1);
        }

        for (int i = 0; i < (int)vertices.size() - 1; i++) {
            adjacencyList[i].insert(i + 1);
            adjacencyList[i + 1].insert(i);
        }
        adjacencyList.back().insert(0);
        adjacencyList[0].insert(vertices.size() - 1);
    }

public:
    Coloring(const std::vector<Point>& verts, const std::vector<Triangle>& tris)
        : vertexColors(verts.size(), UNCOLORED), vertices(verts), triangles(tris) {}

    bool colorTriangulation() {
        if (triangles.empty()) {
            std::cerr << "No triangles to color\n";
            return false;
        }

        buildAdjacencyGraph();
        vertexColors.assign(vertices.size(), UNCOLORED);

        std::queue<int> triangleQueue;
        std::set<int> processedTriangles;

        triangleQueue.push(0);
        processedTriangles.insert(0);

        const Triangle& firstTriangle = triangles[0];
        vertexColors[firstTriangle.v0] = RED;
        vertexColors[firstTriangle.v1] = GREEN;
        vertexColors[firstTriangle.v2] = BLUE;

        while (!triangleQueue.empty()) {
            int currentTriIdx = triangleQueue.front();
            triangleQueue.pop();

            const Triangle& currentTri = triangles[currentTriIdx];

            for (int i = 0; i < (int)triangles.size(); i++) {
                if (processedTriangles.count(i)) continue;

                if (!currentTri.sharesEdge(triangles[i])) continue;

                const Triangle& nextTri = triangles[i];

                std::vector<int> sharedVertices;
                for (int v : {nextTri.v0, nextTri.v1, nextTri.v2}) {
                    if (currentTri.contains(v)) {
                        sharedVertices.push_back(v);
                    }
                }

                if (sharedVertices.size() != 2) continue;

                int sharedV1 = sharedVertices[0];
                int sharedV2 = sharedVertices[1];

                int unsharedV = -1;
                if (!currentTri.contains(nextTri.v0)) unsharedV = nextTri.v0;
                else if (!currentTri.contains(nextTri.v1)) unsharedV = nextTri.v1;
                else unsharedV = nextTri.v2;

                if (unsharedV == -1) continue;

                Color color1 = vertexColors[sharedV1];
                Color color2 = vertexColors[sharedV2];

                if (color1 == UNCOLORED || color2 == UNCOLORED) continue;

                Color requiredColor = UNCOLORED;
                for (int c = 0; c < 3; c++) {
                    if (c != color1 && c != color2) {
                        requiredColor = (Color)c;
                        break;
                    }
                }

                vertexColors[unsharedV] = requiredColor;

                processedTriangles.insert(i);
                triangleQueue.push(i);
            }
        }

        return verifyColoring();
    }

    bool verifyColoring() const {
        for (const auto& triangle : triangles) {
            Color c0 = vertexColors[triangle.v0];
            Color c1 = vertexColors[triangle.v1];
            Color c2 = vertexColors[triangle.v2];

            if (c0 == UNCOLORED || c1 == UNCOLORED || c2 == UNCOLORED) {
                std::cerr << "Uncolored vertex in triangle\n";
                return false;
            }

            if (!(c0 != c1 && c1 != c2 && c0 != c2)) {
                std::cerr << "Invalid coloring: adjacent vertices share color\n";
                return false;
            }

            std::set<Color> colors = {c0, c1, c2};
            if (colors.size() != 3) {
                std::cerr << "Triangle does not contain all three colors\n";
                return false;
            }
        }

        return true;
    }

    const std::vector<Color>& getColors() const {
        return vertexColors;
    }

    Color getVertexColor(int i) const {
        return vertexColors[i];
    }

    std::vector<int> getColorClass(Color color) const {
        std::vector<int> result;
        for (int i = 0; i < (int)vertexColors.size(); i++) {
            if (vertexColors[i] == color) {
                result.push_back(i);
            }
        }
        return result;
    }

    int getColorClassSize(Color color) const {
        return getColorClass(color).size();
    }

    std::string colorToString(Color c) const {
        switch (c) {
            case RED: return "Red";
            case GREEN: return "Green";
            case BLUE: return "Blue";
            default: return "Uncolored";
        }
    }

    void printColorCounts() const {
        std::cout << "Color distribution:\n";
        std::cout << "  Red:   " << getColorClassSize(RED) << "\n";
        std::cout << "  Green: " << getColorClassSize(GREEN) << "\n";
        std::cout << "  Blue:  " << getColorClassSize(BLUE) << "\n";
    }
};

#endif
