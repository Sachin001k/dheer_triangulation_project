#ifndef GUARDSET_H
#define GUARDSET_H

#include "Coloring.h"
#include "Triangle.h"
#include <vector>
#include <algorithm>
#include <cmath>

class GuardSet {
private:
    std::vector<int> guards;
    const std::vector<Point>& vertices;
    const std::vector<Triangle>& triangles;
    const Coloring& coloring;
    bool coverageVerified;

public:
    GuardSet(const std::vector<Point>& verts, const std::vector<Triangle>& tris,
             const Coloring& col)
        : vertices(verts), triangles(tris), coloring(col), coverageVerified(false) {}

    bool selectGuards() {
        Color colors[] = {RED, GREEN, BLUE};
        int minSize = std::numeric_limits<int>::max();
        Color selectedColor = RED;

        for (Color color : colors) {
            int size = coloring.getColorClassSize(color);
            if (size < minSize) {
                minSize = size;
                selectedColor = color;
            }
        }

        guards = coloring.getColorClass(selectedColor);

        if (guards.empty()) {
            std::cerr << "Failed to select guards\n";
            return false;
        }

        return true;
    }

    bool verifyCoverage() {
        coverageVerified = true;

        for (const auto& triangle : triangles) {
            bool hasGuard = false;

            if (std::find(guards.begin(), guards.end(), triangle.v0) != guards.end()) {
                hasGuard = true;
            } else if (std::find(guards.begin(), guards.end(), triangle.v1) != guards.end()) {
                hasGuard = true;
            } else if (std::find(guards.begin(), guards.end(), triangle.v2) != guards.end()) {
                hasGuard = true;
            }

            if (!hasGuard) {
                std::cerr << "Triangle not covered by any guard\n";
                coverageVerified = false;
                return false;
            }
        }

        return true;
    }

    const std::vector<int>& getGuards() const {
        return guards;
    }

    int getGuardCount() const {
        return guards.size();
    }

    int getTheoreticalBound() const {
        return (int)std::floor(vertices.size() / 3.0);
    }

    bool satisfiesBound() const {
        return getGuardCount() <= getTheoreticalBound();
    }

    bool isGuard(int vertexIdx) const {
        return std::find(guards.begin(), guards.end(), vertexIdx) != guards.end();
    }

    void printInfo() const {
        std::cout << "\nGuard Set Information:\n";
        std::cout << "  Guards placed: " << getGuardCount() << "\n";
        std::cout << "  Theoretical bound: " << getTheoreticalBound() << "\n";
        std::cout << "  Bound satisfied: " << (satisfiesBound() ? "Yes" : "No") << "\n";
        std::cout << "  Coverage verified: " << (coverageVerified ? "Yes" : "No") << "\n";
        std::cout << "  Guard vertices: ";
        for (int g : guards) {
            std::cout << g << " ";
        }
        std::cout << "\n";
    }
};

#endif
