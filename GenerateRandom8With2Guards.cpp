#include "PolygonGenerator.h"
#include "Polygon.h"
#include "ArtGalleryProblem.h"
#include "SVGVisualizer.h"
#include <iostream>
#include <vector>

/**
 * Generate random 8-point polygon with exactly 2 guards
 * Keeps generating until a polygon produces 2 guards from coloring
 */
std::vector<Point> generateRandom8PointsWith2Guards() {
    std::cout << "\n🎲 Generating Random 8-Point Polygon with 2 Guards...\n";
    std::cout << "════════════════════════════════════════════════════════════════\n";
    
    int attempt = 0;
    const int MAX_ATTEMPTS = 100;
    
    while(attempt < MAX_ATTEMPTS) {
        attempt++;
        
        // Generate random 8 points in [-100, 100] x [-100, 100]
        std::vector<Point> vertices = PolygonGenerator::generateRandomSimplePolygon(8, -100.0, 100.0);
        
        // Create polygon and solve
        Polygon polygon(vertices);
        if(!polygon.isValid()) {
            continue;
        }
        
        ArtGalleryProblem agp(vertices);
        if(!agp.solve()) {
            continue;
        }
        
        // Check if this polygon has exactly 2 guards
        int guardCount = agp.getGuardCount();
        int bound = agp.getTheoreticalBound();
        
        if(guardCount == 2) {
            std::cout << "✅ Found Random 8 with 2 Guards after " << attempt << " attempts!\n\n";
            std::cout << "📊 Configuration Details:\n";
            std::cout << "   Vertices: 8\n";
            std::cout << "   Triangles: " << (vertices.size() - 2) << "\n";
            std::cout << "   Guards: " << guardCount << "\n";
            std::cout << "   Bound: ⌊8/3⌋ = " << bound << "\n";
            std::cout << "   Status: " << guardCount << " ≤ " << bound << " ✅\n";
            
            // Print color distribution
            const Coloring& coloring = agp.getColoring();
            std::cout << "\n🎨 Color Distribution:\n";
            std::cout << "   🔴 Red:   " << coloring.getColorClassSize(RED) << " vertices\n";
            std::cout << "   🟢 Green: " << coloring.getColorClassSize(GREEN) << " vertices\n";
            std::cout << "   🔵 Blue:  " << coloring.getColorClassSize(BLUE) << " vertices\n";
            
            // Print vertices for reference
            std::cout << "\n📍 Vertex Coordinates:\n";
            for(int i = 0; i < 8; i++) {
                std::cout << "   Vertex " << i << ": (" << vertices[i].x << ", " << vertices[i].y << ")\n";
            }
            
            std::cout << "\n════════════════════════════════════════════════════════════════\n";
            
            return vertices;
        }
    }
    
    std::cout << "⚠️  Could not find Random 8 with 2 guards after " << MAX_ATTEMPTS 
              << " attempts.\n";
    std::cout << "   (This is normal - guard count depends on point distribution)\n";
    std::cout << "   (Any random 8 polygon is valid, regardless of guard count)\n\n";
    
    // Return a valid random 8 even if not exactly 2 guards
    return PolygonGenerator::generateRandomSimplePolygon(8, -100.0, 100.0);
}

// Main function to test
int main() {
    std::vector<Point> random8 = generateRandom8PointsWith2Guards();
    
    // Create polygon and visualize
    Polygon polygon(random8);
    ArtGalleryProblem agp(random8);
    agp.solve();
    
    // Visualize to HTML
    agp.visualizeSolution("random8_with_2guards.html");
    std::cout << "✓ Visualization saved to: random8_with_2guards.html\n";
    
    return 0;
}
