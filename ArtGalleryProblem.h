#ifndef ARTGALLERYPROBLEM_H
#define ARTGALLERYPROBLEM_H

#include "Polygon.h"
#include "Triangulation.h"
#include "Coloring.h"
#include "GuardSet.h"
#include "SVGVisualizer.h"
#include <chrono>
#include <iostream>

class ArtGalleryProblem {
private:
    Polygon polygon;
    Triangulation* triangulation;
    Coloring* coloring;
    GuardSet* guardSet;
    SVGVisualizer visualizer;

    bool triangulationSuccess;
    bool coloringSuccess;
    bool guardSelectionSuccess;
    bool coverageSuccess;

    std::chrono::duration<double, std::milli> computationTime;

public:
    ArtGalleryProblem(const std::vector<Point>& vertices)
        : polygon(vertices), triangulation(nullptr), coloring(nullptr),
          guardSet(nullptr), visualizer(polygon),
          triangulationSuccess(false), coloringSuccess(false),
          guardSelectionSuccess(false), coverageSuccess(false) {}

    ~ArtGalleryProblem() {
        delete triangulation;
        delete coloring;
        delete guardSet;
    }

    bool solve() {
        auto startTime = std::chrono::high_resolution_clock::now();

        std::cout << "\n=== Art Gallery Problem Solver ===\n\n";

        polygon.printInfo();

        if (!polygon.isValid()) {
            std::cerr << "\nPolygon validation failed!\n";
            return false;
        }

        std::cout << "\n[Step 1] Triangulating polygon using ear-clipping...\n";
        triangulation = new Triangulation(polygon);
        triangulationSuccess = triangulation->triangulate();

        if (!triangulationSuccess) {
            std::cerr << "Triangulation failed!\n";
            return false;
        }

        std::cout << "✓ Triangulation successful\n";
        std::cout << "  Triangles: " << triangulation->getTriangleCount() << "\n";

        std::cout << "\n[Step 2] Three-coloring the triangulation...\n";
        coloring = new Coloring(polygon.getVertices(), triangulation->getTriangles());
        coloringSuccess = coloring->colorTriangulation();

        if (!coloringSuccess) {
            std::cerr << "Coloring failed!\n";
            return false;
        }

        std::cout << "✓ Coloring successful\n";
        coloring->printColorCounts();

        std::cout << "\n[Step 3] Selecting guards from smallest color class...\n";
        guardSet = new GuardSet(polygon.getVertices(), triangulation->getTriangles(), *coloring);
        guardSelectionSuccess = guardSet->selectGuards();

        if (!guardSelectionSuccess) {
            std::cerr << "Guard selection failed!\n";
            return false;
        }

        std::cout << "✓ Guard selection successful\n";

        std::cout << "\n[Step 4] Verifying coverage...\n";
        coverageSuccess = guardSet->verifyCoverage();

        if (!coverageSuccess) {
            std::cerr << "Coverage verification failed!\n";
            return false;
        }

        std::cout << "✓ Coverage verified\n";

        if (!guardSet->satisfiesBound()) {
            std::cerr << "Warning: Guard count exceeds theoretical bound!\n";
            return false;
        }

        std::cout << "✓ Guard count satisfies theoretical bound\n";

        auto endTime = std::chrono::high_resolution_clock::now();
        computationTime = endTime - startTime;

        printResults();

        return true;
    }

    bool visualizeSolution(const std::string& filename) {
        if (!guardSelectionSuccess) {
            std::cerr << "Cannot visualize: solution not found\n";
            return false;
        }

        visualizer.setTriangulation(triangulation->getTriangles());
        visualizer.setColoring(*coloring);
        visualizer.setGuardSet(*guardSet);

        return visualizer.visualize(filename);
    }

    void printResults() const {
        std::cout << "\n=== RESULTS ===\n";
        std::cout << "Polygon vertices: " << polygon.getVertexCount() << "\n";
        std::cout << "Triangles: " << triangulation->getTriangleCount() << "\n";
        guardSet->printInfo();
        std::cout << "\nComputation time: " << std::fixed << std::setprecision(3)
                  << computationTime.count() << " ms\n";
    }

    int getGuardCount() const {
        return guardSet ? guardSet->getGuardCount() : -1;
    }

    int getTheoreticalBound() const {
        return guardSet ? guardSet->getTheoreticalBound() : -1;
    }

    bool isSuccessful() const {
        return triangulationSuccess && coloringSuccess &&
               guardSelectionSuccess && coverageSuccess;
    }

    const std::vector<Triangle>& getTriangulation() const {
        return triangulation->getTriangles();
    }

    const Coloring& getColoring() const {
        return *coloring;
    }

    const GuardSet& getGuardSet() const {
        return *guardSet;
    }

    const Polygon& getPolygon() const {
        return polygon;
    }

    double getComputationTime() const {
        return computationTime.count();
    }
};

#endif
