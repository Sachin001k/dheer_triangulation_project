#include "ArtGalleryProblem.h"
#include "PolygonGenerator.h"
#include "CombinedVisualizer.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <climits>

CombinedVisualizer visualizer("output.log");

struct TestInfo {
    std::vector<Point> vertices;
    std::string name;
    std::string outputFile;
};

// Generate random 8-point polygon with 2 guards (try up to 100 times)
std::vector<Point> generateRandom8With2Guards() {
    std::cout << "\n🎲 Attempting to generate Random 8 with 2 Guards...\n";

    for(int attempt = 0; attempt < 100; attempt++) {
        std::vector<Point> vertices = PolygonGenerator::generateRandomSimplePolygon(8, -100.0, 100.0);
        ArtGalleryProblem agp(vertices);

        if(agp.solve() && agp.getGuardCount() == 2) {
            std::cout << "✅ Found Random 8 with 2 Guards on attempt " << (attempt + 1) << "!\n";
            return vertices;
        }
    }

    // If not found, return a regular random 8
    std::cout << "ℹ️  No 2-guard config found in 100 attempts (normal)\n";
    std::cout << "   Returning standard random 8 polygon...\n";
    return PolygonGenerator::generateRandomSimplePolygon(8, -100.0, 100.0);
}

TestResult testPolygon(const TestInfo& info) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "Testing: " << info.name << "\n";
    std::cout << std::string(60, '=') << "\n";

    TestResult result;
    result.name = info.name;
    result.htmlFile = info.outputFile;
    result.success = false;

    ArtGalleryProblem agp(info.vertices);

    if (agp.solve()) {
        std::cout << "\n✓ Solution found successfully!\n";

        if (agp.visualizeSolution(info.outputFile)) {
            std::cout << "✓ Visualization saved to: " << info.outputFile << "\n";

            result.vertices = info.vertices.size();
            result.success = true;
        } else {
            std::cerr << "✗ Failed to generate visualization\n";
        }
    } else {
        std::cerr << "\n✗ Failed to solve the problem for this polygon\n";
    }

    return result;
}

int main() {
    std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║      Art Gallery Problem Implementation                    ║\n";
    std::cout << "║     Polygon Triangulation & Three-Coloring                 ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";

    std::vector<TestInfo> testCases = {
        {PolygonGenerator::generateTriangle(100),
         "Simple Triangle",
         "triangle.html"},

        {PolygonGenerator::generateRectangle(150, 100),
         "Rectangle",
         "rectangle.html"},

        {PolygonGenerator::generatePentagon(100),
         "Pentagon",
         "pentagon.html"},

        {PolygonGenerator::generateHexagon(100),
         "Hexagon",
         "hexagon.html"},

        {PolygonGenerator::generateOctagon(100),
         "Octagon",
         "octagon.html"},

        {PolygonGenerator::generateComplexPolygon(),
         "Complex Polygon (14 vertices)",
         "complex.html"},

        {PolygonGenerator::generateRandomStarPolygon(6, 120, 50),
         "Star Polygon (12 vertices)",
         "star.html"},

        {PolygonGenerator::generateRandomSimplePolygon(8, -100.0, 100.0),
         "Random Simple Polygon (8 vertices)",
         "random8.html"},

        {PolygonGenerator::generateRandomSimplePolygon(12, -100.0, 100.0),
         "Random Simple Polygon (12 vertices)",
         "random12.html"},

        {generateRandom8With2Guards(),
         "Random Simple Polygon (8 vertices with 2 Guards)",
         "random8_2guards.html"},

        {PolygonGenerator::generateRandomSimplePolygon(20, -100.0, 100.0),
         "Random Simple Polygon (20 vertices)",
         "random20.html"}
    };

    int successCount = 0;
    int totalCount = testCases.size();
    std::vector<TestResult> allResults;

    // Run all tests and collect detailed results
    for (const auto& testCase : testCases) {
        ArtGalleryProblem agp(testCase.vertices);

        auto startTime = std::chrono::high_resolution_clock::now();
        bool solved = agp.solve();
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime);

        if (solved) {
            successCount++;
            if (agp.visualizeSolution(testCase.outputFile)) {
                std::cout << "✓ Visualization saved to: " << testCase.outputFile << "\n";
            }

            TestResult result;
            result.name = testCase.name;
            result.vertices = testCase.vertices.size();
            result.triangles = testCase.vertices.size() - 2;
            result.guardCount = agp.getGuardCount();
            result.theoreticalBound = agp.getTheoreticalBound();
            result.executionTime = duration.count();
            result.success = true;
            result.htmlFile = testCase.outputFile;

            // Extract actual color counts from the coloring
            result.redCount = agp.getColoring().getColorClassSize(RED);
            result.greenCount = agp.getColoring().getColorClassSize(GREEN);
            result.blueCount = agp.getColoring().getColorClassSize(BLUE);

            allResults.push_back(result);
        }
    }

    // Save combined HTML and log
    for (const auto& result : allResults) {
        visualizer.addResult(result);
    }

    visualizer.saveCombinedHTML("all_results.html");
    visualizer.savePerformanceLog();

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "SUMMARY\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "Total test cases: " << totalCount << "\n";
    std::cout << "Successful: " << successCount << "\n";
    std::cout << "Failed: " << (totalCount - successCount) << "\n";
    std::cout << "Success rate: " << std::fixed << std::setprecision(1)
              << (100.0 * successCount / totalCount) << "%\n";

    std::cout << "\n✓ Generated visualization files:\n";
    for (const auto& testCase : testCases) {
        std::cout << "  - " << testCase.outputFile << " (" << testCase.name << ")\n";
    }

    std::cout << "\n✓ Combined Results:\n";
    std::cout << "  - all_results.html (All tests in one page)\n";
    std::cout << "  - output.log (Performance metrics & analysis)\n";

    std::cout << "\nOpen the files in a web browser to view results:\n";
    std::cout << "  open all_results.html\n\n";

    return 0;
}
