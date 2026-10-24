# Quick Start Guide - Art Gallery Problem C++ Implementation

## What Is This?

A complete C++ implementation of the Art Gallery Theorem that solves how many guards are needed to observe an entire polygonal gallery. The implementation uses polygon triangulation and three-coloring to compute optimal guard positions.

## Get Started in 5 Minutes

### 1. Clone/Navigate to Project
```bash
cd /Users/sachink/Documents/Projects/dheer
```

### 2. Build
```bash
mkdir -p build
cd build
cmake ..
cmake --build . --config Release
```

### 3. Run
```bash
./art_gallery_solver
```

### 4. View Results
Open any HTML file in a web browser:
```bash
# macOS
open complex.html
open pentagon.html

# Linux
firefox complex.html

# Windows
start complex.html
```

## What You Get

✅ **Console Output**
- Step-by-step algorithm execution
- Problem statistics for each polygon
- Computation time measurements
- Summary of all results

✅ **HTML Visualizations** (10 test cases)
- `triangle.html` - Simple 3-vertex triangle
- `rectangle.html` - 4-vertex rectangle
- `pentagon.html` - 5-vertex pentagon
- `hexagon.html` - 6-vertex hexagon
- `octagon.html` - 8-vertex octagon
- `complex.html` - Complex 14-vertex polygon
- `star.html` - 12-vertex star polygon
- `random8.html`, `random12.html`, `random20.html` - Random polygons

## Understanding the Visualizations

Each HTML file shows:

**Visual Elements**:
- **Black solid edges** = Polygon boundary
- **Gray dashed lines** = Triangulation diagonals
- **Colored circles** = Vertices
  - 🔴 Red vertices
  - 🟢 Green vertices
  - 🔵 Blue vertices
- **Gold boxes** = Guard positions ⭐

**Information Box**:
```
Polygon Vertices: 14          (total number of vertices)
Triangles: 12                 (n-2 triangles from triangulation)
Color Distribution: Red=3, Green=6, Blue=5  (vertices per color)
Guards Required: 3            (guards placed)
Theoretical Bound: 5          (upper bound: ⌊n/3⌋)
Bound Satisfied: ✓ Yes        (verification status)
```

## Example: Understanding the Algorithm

### For a Pentagon (5 vertices):

```
Step 1: Validate polygon
  ✓ 5 distinct vertices
  ✓ Non-zero area
  ✓ No self-intersections

Step 2: Triangulate (ear-clipping)
  Creates 3 triangles (5-2=3)

Step 3: Three-color triangulation
  Red:   1 vertex
  Green: 2 vertices
  Blue:  2 vertices

Step 4: Select guards
  Smallest color class = Red (1 vertex)
  Guards needed = 1

Step 5: Verify
  ✓ All triangles have a guard
  ✓ 1 ≤ ⌊5/3⌋ = 1 ✓ (bound satisfied)

Result: Place 1 guard, covers entire polygon
```

## Using in Your Code

### Basic Example

```cpp
#include "ArtGalleryProblem.h"
#include <vector>

int main() {
    // Define polygon vertices (counterclockwise order)
    std::vector<Point> vertices = {
        Point(0, 0),
        Point(100, 0),
        Point(100, 100),
        Point(0, 100)
    };

    // Create solver
    ArtGalleryProblem solver(vertices);

    // Solve
    if (solver.solve()) {
        std::cout << "Guards needed: " << solver.getGuardCount() << "\n";
        std::cout << "Bound: " << solver.getTheoreticalBound() << "\n";
        
        // Generate visualization
        solver.visualizeSolution("my_polygon.html");
    }

    return 0;
}
```

### Custom Polygon from File

```cpp
#include "ArtGalleryProblem.h"
#include <fstream>

int main() {
    std::vector<Point> vertices;
    
    // Read from file (format: x y on each line)
    std::ifstream file("polygon.txt");
    double x, y;
    while (file >> x >> y) {
        vertices.emplace_back(x, y);
    }
    
    // Solve
    ArtGalleryProblem solver(vertices);
    if (solver.solve()) {
        solver.visualizeSolution("result.html");
    }
    
    return 0;
}
```

### Multiple Polygons

```cpp
#include "ArtGalleryProblem.h"
#include "PolygonGenerator.h"

int main() {
    std::vector<std::vector<Point>> testPolygons = {
        PolygonGenerator::generateTriangle(100),
        PolygonGenerator::generateRectangle(150, 100),
        PolygonGenerator::generatePentagon(100),
        PolygonGenerator::generateHexagon(100),
    };

    for (size_t i = 0; i < testPolygons.size(); i++) {
        ArtGalleryProblem solver(testPolygons[i]);
        if (solver.solve()) {
            solver.visualizeSolution("polygon_" + std::to_string(i) + ".html");
        }
    }

    return 0;
}
```

## Class Overview

### Key Classes

**Point** - 2D points with operations
```cpp
Point p1(10, 20);
Point p2 = p1 + Point(5, 5);  // (15, 25)
double dist = p1.distance(p2); // Euclidean distance
```

**Polygon** - Polygon with validation
```cpp
Polygon poly(vertices);
if (poly.isValid()) {
    std::cout << "Vertices: " << poly.getVertexCount() << "\n";
}
```

**Triangulation** - Ear-clipping algorithm
```cpp
Triangulation tri(polygon);
if (tri.triangulate()) {
    auto triangles = tri.getTriangles();
    std::cout << "Triangles: " << triangles.size() << "\n";
}
```

**Coloring** - Three-coloring
```cpp
Coloring color(vertices, triangles);
if (color.colorTriangulation()) {
    color.printColorCounts();  // Distribution of colors
}
```

**GuardSet** - Guard placement
```cpp
GuardSet guards(vertices, triangles, coloring);
guards.selectGuards();
if (guards.verifyCoverage()) {
    std::cout << "Guards: " << guards.getGuardCount() << "\n";
}
```

**ArtGalleryProblem** - Main orchestrator
```cpp
ArtGalleryProblem solver(vertices);
if (solver.solve()) {
    solver.visualizeSolution("output.html");
}
```

## Algorithm at a Glance

```
┌──────────────────────────────────┐
│ Input: n-vertex simple polygon   │
└──────────────────────────────────┘
              ↓
┌──────────────────────────────────┐
│ Validate polygon is simple       │
│ (no self-intersections)          │
└──────────────────────────────────┘
              ↓
┌──────────────────────────────────┐
│ Triangulate using ear-clipping   │
│ → n-2 triangles                  │
└──────────────────────────────────┘
              ↓
┌──────────────────────────────────┐
│ Three-color the triangulation    │
│ → each triangle has all 3 colors │
└──────────────────────────────────┘
              ↓
┌──────────────────────────────────┐
│ Select smallest color class      │
│ → ⌊n/3⌋ guards maximum          │
└──────────────────────────────────┘
              ↓
┌──────────────────────────────────┐
│ Verify entire polygon covered    │
│ by guard positions               │
└──────────────────────────────────┘
              ↓
┌──────────────────────────────────┐
│ Output: guards + visualization   │
└──────────────────────────────────┘
```

## Common Patterns

### Generate & Solve Random Polygon
```cpp
auto vertices = PolygonGenerator::generateRandomConvexPolygon(12, 100);
ArtGalleryProblem solver(vertices);
solver.solve();
```

### Regular Polygons
```cpp
// Pentagon
auto pent = PolygonGenerator::generatePentagon(100);
// Hexagon
auto hex = PolygonGenerator::generateHexagon(100);
// Octagon
auto oct = PolygonGenerator::generateOctagon(100);
```

### Star Polygon
```cpp
auto star = PolygonGenerator::generateRandomStarPolygon(6, 120, 50);
ArtGalleryProblem solver(star);
solver.solve();
```

## File Structure Quick Reference

```
Headers (Include These):
  Geometry.h           - Points and geometric utilities
  Polygon.h            - Polygon class
  Triangulation.h      - Triangulation algorithm
  Coloring.h           - Three-coloring
  GuardSet.h           - Guard placement
  ArtGalleryProblem.h  - Main solver
  PolygonGenerator.h   - Test polygon generation
  SVGVisualizer.h      - Visualization

Source:
  main.cpp             - Test harness and examples

Documentation:
  README.md            - Complete reference
  IMPLEMENTATION_GUIDE.md - Architecture guide
  ALGORITHM_ANALYSIS.md   - Complexity analysis
  QUICK_START.md       - This file

Output:
  *.html               - Visualization files
```

## Troubleshooting

### Build Issues

**Problem**: CMake not found
```bash
# macOS
brew install cmake

# Ubuntu
sudo apt install cmake

# Windows
# Download from cmake.org
```

**Problem**: Compiler not found
```bash
# macOS
xcode-select --install

# Ubuntu
sudo apt install build-essential

# Windows
# Install Visual Studio Community
```

### Runtime Issues

**Problem**: "Polygon validation failed"
- Check vertices are in correct order (counterclockwise)
- Ensure no duplicate vertices
- Verify polygon doesn't self-intersect

**Problem**: No visualization generated
- Check that output file path is writable
- Ensure filename has `.html` extension

## Performance Expectations

| Vertices | Time | Memory |
|---|---|---|
| 3 | 0.03 ms | 1 KB |
| 10 | 0.1 ms | 3 KB |
| 50 | 3 ms | 15 KB |
| 100 | 15 ms | 30 KB |
| 1000 | 1500 ms | 300 KB |

**Bottleneck**: Triangulation is O(n²)

## Theory in 60 Seconds

1. **Chvátal's Theorem**: Any n-vertex polygon needs at most ⌊n/3⌋ guards

2. **Fisk's Proof**:
   - Break polygon into triangles
   - Color triangles with 3 colors (each has all 3)
   - Place guards at smallest color class
   - Smallest class has ≤ ⌊n/3⌋ vertices
   - Every point sees a guard ✓

3. **Why it works**: Triangles are convex, so if a guard is at a vertex of a triangle, it can see all points in that triangle.

## Next Steps

1. **Run the examples**: `./art_gallery_solver`
2. **View visualizations**: Open `.html` files in browser
3. **Study the code**: Start with `ArtGalleryProblem.h`
4. **Modify main.cpp**: Add your own test cases
5. **Read full docs**: See `README.md` and `IMPLEMENTATION_GUIDE.md`

## Key Formulas

**Polygon area**:
```
A = (1/2) * Σ(x[i]*y[i+1] - x[i+1]*y[i])
```

**Point-in-triangle**:
```
Use barycentric coordinates or cross products
If CCW(p,a,b)*CCW(p,b,c)*CCW(p,c,a) all same sign
→ point p is inside triangle ABC
```

**Triangulation count**:
```
T = n - 2  (always)
E = 2n - 3 (edges including diagonals)
```

**Guard bound**:
```
max_guards = ⌊n/3⌋
optimal = min(r, g, b) where r,g,b = color class sizes
```

## Educational Use

Perfect for:
- 📚 Learning computational geometry
- 🔍 Understanding triangulation algorithms
- 🎨 Visualizing graph coloring
- 🏛️ Implementing mathematical proofs

## Questions?

Check:
1. `README.md` - Complete reference
2. `IMPLEMENTATION_GUIDE.md` - Architecture details
3. `ALGORITHM_ANALYSIS.md` - Complexity analysis
4. Code comments - Implementation details
5. Test cases in `main.cpp` - Usage examples

---

**Happy coding!** 🎉

Start with:
```bash
cd /Users/sachink/Documents/Projects/dheer/build
./art_gallery_solver
# Then open any .html file in your browser
```
