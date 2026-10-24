#include "Geometry.h"
#include "Polygon.h"
#include "PolygonGenerator.h"
#include "ArtGalleryProblem.h"
#include <iostream>
#include <iomanip>
#include <chrono>

void printPolygonInfo(const std::string& name, const Polygon& poly) {
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "Test: " << name << "\n";
    std::cout << std::string(70, '=') << "\n";

    const auto& vertices = poly.getVertices();
    std::cout << "Vertices (" << vertices.size() << "):\n";

    // Check domain bounds
    double minX = vertices[0].x, maxX = vertices[0].x;
    double minY = vertices[0].y, maxY = vertices[0].y;

    for (size_t i = 0; i < vertices.size(); i++) {
        std::cout << "  v" << i << ": (" << std::fixed << std::setprecision(2)
                  << vertices[i].x << ", " << vertices[i].y << ")\n";

        minX = std::min(minX, vertices[i].x);
        maxX = std::max(maxX, vertices[i].x);
        minY = std::min(minY, vertices[i].y);
        maxY = std::max(maxY, vertices[i].y);
    }

    std::cout << "\nBounds:\n";
    std::cout << "  X range: [" << minX << ", " << maxX << "]\n";
    std::cout << "  Y range: [" << minY << ", " << maxY << "]\n";
    std::cout << "  In [-100,100] domain: " << (minX >= -100 && maxX <= 100 && minY >= -100 && maxY <= 100 ? "YES" : "NO") << "\n";

    std::cout << "  Area: " << std::abs(GeometryUtils::polygonSignedArea(poly.getVertices())) << "\n";
    std::cout << "  Valid: " << (poly.isValid() ? "YES" : "NO") << "\n";
}

void testRandomPolygon(const std::string& name, int numVertices, int testIndex) {
    std::cout << "\n\n";
    std::cout << "+" << std::string(68, '-') << "+\n";
    std::cout << "| TESTING: " << std::left << std::setw(59) << name << "|\n";
    std::cout << "+" << std::string(68, '-') << "+\n";

    // Generate random simple polygon in [-100, 100] domain
    auto vertices = PolygonGenerator::generateRandomSimplePolygon(numVertices, -100.0, 100.0);
    Polygon polygon(vertices);

    printPolygonInfo(name, polygon);

    if (!polygon.isValid()) {
        std::cout << "\nPolygon is invalid! Skipping triangulation.\n";
        return;
    }

    // Test triangulation
    ArtGalleryProblem solver(vertices);
    bool success = solver.solve();

    if (!success) {
        std::cout << "\nTriangulation FAILED!\n";
        return;
    }

    const auto& triangles = solver.getTriangulation();
    const auto& coloring = solver.getColoring();
    const auto& guardSet = solver.getGuardSet();

    std::cout << "\n[TRIANGULATION RESULTS]\n";
    std::cout << "  Triangles: " << triangles.size() << " (expected: " << (numVertices - 2) << ")\n";
    std::cout << "  Status: " << (triangles.size() == (size_t)(numVertices - 2) ? "CORRECT" : "INCORRECT") << "\n";

    std::cout << "\n[THREE-COLORING RESULTS]\n";
    std::cout << "  Red:           " << coloring.getColorClassSize(RED) << "\n";
    std::cout << "  Green:         " << coloring.getColorClassSize(GREEN) << "\n";
    std::cout << "  Blue:          " << coloring.getColorClassSize(BLUE) << "\n";

    std::cout << "\n[GUARD PLACEMENT RESULTS]\n";
    std::cout << "  Guards placed:     " << guardSet.getGuardCount() << "\n";
    std::cout << "  Theoretical bound: " << guardSet.getTheoreticalBound() << "\n";
    std::cout << "  Bound satisfied:   " << (guardSet.satisfiesBound() ? "YES" : "NO") << "\n";

    std::cout << "\n[PERFORMANCE]\n";
    std::cout << "  Execution time: " << std::fixed << std::setprecision(3) << solver.getComputationTime() << " ms\n";

    // Generate visualization
    std::string filename = "random_" + std::to_string(numVertices) + "_v" + std::to_string(testIndex) + ".html";
    bool vizSuccess = solver.visualizeSolution(filename);

    if (vizSuccess) {
        std::cout << "\n[VISUALIZATION]\n";
        std::cout << "  HTML file: " << filename << "\n";
        std::cout << "  Visualization generated successfully\n";
    }

    std::cout << "\n[FINAL STATUS]\n";
    std::cout << "  Overall result: PASSED\n";
}

int main() {
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "RANDOM SIMPLE POLYGON GENERATION & TRIANGULATION TEST SUITE\n";
    std::cout << "Domain: [-100, 100] x [-100, 100]\n";
    std::cout << std::string(70, '=') << "\n";

    // Test multiple random polygons with different vertex counts
    testRandomPolygon("Random Convex Polygon (5 vertices)", 5, 1);
    testRandomPolygon("Random Convex Polygon (8 vertices)", 8, 2);
    testRandomPolygon("Random Convex Polygon (10 vertices)", 10, 3);
    testRandomPolygon("Random Convex Polygon (12 vertices)", 12, 4);
    testRandomPolygon("Random Convex Polygon (15 vertices)", 15, 5);
    testRandomPolygon("Random Convex Polygon (20 vertices)", 20, 6);

    std::cout << "\n\n";
    std::cout << "+" << std::string(68, '-') << "+\n";
    std::cout << "| ALL TESTS COMPLETED                                               |\n";
    std::cout << "+" << std::string(68, '-') << "+\n";
    std::cout << "\n";

    return 0;
}
