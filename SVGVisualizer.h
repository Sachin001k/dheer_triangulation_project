#ifndef SVGVISUALIZER_H
#define SVGVISUALIZER_H

#include "Geometry.h"
#include "Polygon.h"
#include "Triangle.h"
#include "Coloring.h"
#include "GuardSet.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <iomanip>

class SVGVisualizer {
private:
    const Polygon& polygon;
    const std::vector<Triangle>* triangles;
    const Coloring* coloring;
    const GuardSet* guardSet;
    double padding;
    double scale;

    struct BoundingBox {
        double minX, maxX, minY, maxY;

        BoundingBox(const std::vector<Point>& vertices) {
            if (vertices.empty()) {
                minX = maxX = minY = maxY = 0;
                return;
            }

            minX = maxX = vertices[0].x;
            minY = maxY = vertices[0].y;

            for (const auto& p : vertices) {
                minX = std::min(minX, p.x);
                maxX = std::max(maxX, p.x);
                minY = std::min(minY, p.y);
                maxY = std::max(maxY, p.y);
            }
        }

        double getWidth() const { return maxX - minX; }
        double getHeight() const { return maxY - minY; }
    };

    std::string colorToRGB(Color c) const {
        switch (c) {
            case RED: return "#FF0000";      // Red (primary color)
            case GREEN: return "#00CC00";    // Pure green
            case BLUE: return "#0066FF";     // Pure blue
            default: return "#CCCCCC";
        }
    }

    std::string colorToName(Color c) const {
        switch (c) {
            case RED: return "Red";
            case GREEN: return "Green";
            case BLUE: return "Blue";
            default: return "Uncolored";
        }
    }

public:
    SVGVisualizer(const Polygon& poly, double pad = 50.0)
        : polygon(poly), triangles(nullptr), coloring(nullptr), guardSet(nullptr),
          padding(pad), scale(1.0) {}

    void setTriangulation(const std::vector<Triangle>& tris) {
        triangles = &tris;
    }

    void setColoring(const Coloring& col) {
        coloring = &col;
    }

    void setGuardSet(const GuardSet& gs) {
        guardSet = &gs;
    }

    bool visualize(const std::string& filename) {
        if (!polygon.isValid()) {
            std::cerr << "Cannot visualize invalid polygon\n";
            return false;
        }

        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Cannot open file: " << filename << "\n";
            return false;
        }

        const auto& vertices = polygon.getVertices();
        BoundingBox bbox(vertices);

        double width = bbox.getWidth() + 2 * padding;
        double height = bbox.getHeight() + 2 * padding;
        scale = 1.0;

        file << "<!DOCTYPE html>\n";
        file << "<html>\n<head>\n";
        file << "<meta charset=\"UTF-8\">\n";
        file << "<title>Art Gallery Problem Visualization</title>\n";
        file << "<style>\n";
        file << "  body { font-family: Arial, sans-serif; margin: 20px; }\n";
        file << "  svg { border: 1px solid #ccc; background-color: #f9f9f9; }\n";
        file << "  .info { margin-top: 20px; padding: 10px; background-color: #f0f0f0; }\n";
        file << "</style>\n</head>\n<body>\n";

        file << "<h1>Art Gallery Problem - Triangulation & Guard Placement</h1>\n";
        file << "<svg width=\"" << width << "\" height=\"" << height << "\">\n";

        file << "<defs>\n";
        file << "  <style>\n";
        file << "    .polygon-edge { stroke: #000; stroke-width: 2.5; fill: none; }\n";
        file << "    .diagonal { stroke: #999; stroke-width: 1.5; fill: none; stroke-dasharray: 5,5; opacity: 0.7; }\n";
        file << "    .vertex { fill: #333 !important; stroke: #000 !important; stroke-width: 1.5 !important; }\n";
        file << "    .vertex-red { }\n";
        file << "    .vertex-green { }\n";
        file << "    .vertex-blue { }\n";
        file << "    .guard-marker { }\n";
        file << "    .vertex-label { font-size: 12px; fill: black; font-weight: bold; font-family: Arial; text-shadow: 1px 1px 2px white; }\n";
        file << "  </style>\n";
        file << "</defs>\n";

        drawTriangulation(file, bbox);
        drawPolygonEdges(file, bbox);
        drawVertices(file, bbox);
        drawGuards(file, bbox);

        file << "</svg>\n";

        file << "<div class=\"info\">\n";
        file << "<h3>Problem Statistics</h3>\n";
        file << "<p><b>Polygon Vertices:</b> " << vertices.size() << "</p>\n";
        if (triangles) {
            file << "<p><b>Triangles:</b> " << triangles->size() << " (expected: " << (vertices.size() - 2) << ")</p>\n";
        }
        if (coloring) {
            file << "<p><b>Color Distribution:</b> ";
            file << "🔴 Red=" << coloring->getColorClassSize(RED) << ", ";
            file << "🟢 Green=" << coloring->getColorClassSize(GREEN) << ", ";
            file << "🔵 Blue=" << coloring->getColorClassSize(BLUE) << "</p>\n";
        }
        if (guardSet) {
            file << "<p><b>Guards Required:</b> " << guardSet->getGuardCount() << "</p>\n";
            file << "<p><b>Theoretical Bound:</b> ⌊" << vertices.size() << "/3⌋ = " << guardSet->getTheoreticalBound() << "</p>\n";
            file << "<p><b>Bound Satisfied:</b> " << (guardSet->satisfiesBound() ? "✓ Yes" : "✗ No") << "</p>\n";
        }
        file << "</div>\n";

        file << "</body>\n</html>\n";

        file.close();
        return true;
    }

private:
    Point transformPoint(const Point& p, const BoundingBox& bbox) const {
        return Point(
            (p.x - bbox.minX) * scale + padding,
            (bbox.maxY - p.y) * scale + padding
        );
    }

    void drawTriangulation(std::ofstream& file, const BoundingBox& bbox) {
        if (!triangles) return;

        const auto& vertices = polygon.getVertices();

        file << "<!-- Triangulation Diagonals -->\n";
        for (const auto& triangle : *triangles) {
            Point p0 = transformPoint(vertices[triangle.v0], bbox);
            Point p1 = transformPoint(vertices[triangle.v1], bbox);
            Point p2 = transformPoint(vertices[triangle.v2], bbox);

            file << "<line x1=\"" << p0.x << "\" y1=\"" << p0.y
                 << "\" x2=\"" << p1.x << "\" y2=\"" << p1.y << "\" class=\"diagonal\"/>\n";
            file << "<line x1=\"" << p1.x << "\" y1=\"" << p1.y
                 << "\" x2=\"" << p2.x << "\" y2=\"" << p2.y << "\" class=\"diagonal\"/>\n";
            file << "<line x1=\"" << p2.x << "\" y1=\"" << p2.y
                 << "\" x2=\"" << p0.x << "\" y2=\"" << p0.y << "\" class=\"diagonal\"/>\n";

            if (coloring) {
                drawColoredTriangle(file, p0, p1, p2, triangle);
            }
        }
    }

    void drawColoredTriangle(std::ofstream& file, const Point&, const Point&,
                             const Point&, const Triangle& triangle) {
        Color c0 = coloring->getVertexColor(triangle.v0);
        Color c1 = coloring->getVertexColor(triangle.v1);
        Color c2 = coloring->getVertexColor(triangle.v2);

        file << "<!-- Triangle with colors: " << colorToName(c0) << ", "
             << colorToName(c1) << ", " << colorToName(c2) << " -->\n";
    }

    void drawPolygonEdges(std::ofstream& file, const BoundingBox& bbox) {
        const auto& vertices = polygon.getVertices();
        int n = vertices.size();

        file << "<!-- Polygon Edges -->\n";
        for (int i = 0; i < n; i++) {
            Point p1 = transformPoint(vertices[i], bbox);
            Point p2 = transformPoint(vertices[(i + 1) % n], bbox);

            file << "<line x1=\"" << p1.x << "\" y1=\"" << p1.y
                 << "\" x2=\"" << p2.x << "\" y2=\"" << p2.y
                 << "\" class=\"polygon-edge\"/>\n";
        }
    }

    void drawVertices(std::ofstream& file, const BoundingBox& bbox) {
        const auto& vertices = polygon.getVertices();

        file << "<!-- Vertices -->\n";
        for (int i = 0; i < (int)vertices.size(); i++) {
            Point p = transformPoint(vertices[i], bbox);
            std::string fillColor = "#333";
            std::string strokeColor = "#000";
            std::string strokeWidth = "1.5";
            std::string circleClass = "vertex";
            double radius = 8;
            bool drawGuardSquare = false;

            if (coloring) {
                Color c = coloring->getVertexColor(i);
                if (c == RED) {
                    fillColor = "#FF0000";      // Red color
                    strokeColor = "#CC0000";    // Darker red stroke
                    circleClass = "vertex-red";
                } else if (c == GREEN) {
                    fillColor = "#00CC00";
                    strokeColor = "#009900";
                    circleClass = "vertex-green";
                } else if (c == BLUE) {
                    fillColor = "#0066FF";
                    strokeColor = "#003399";
                    circleClass = "vertex-blue";
                }
                strokeWidth = "2.5";

                // If this vertex is a guard, add visual indicator
                if (guardSet && guardSet->isGuard(i)) {
                    drawGuardSquare = true;
                    radius = 9;  // Slightly larger for guards
                }
            }

            // Draw border outline for guard vertices
            if (drawGuardSquare) {
                double squareSize = 20;
                file << "<rect x=\"" << (p.x - squareSize/2) << "\" y=\"" << (p.y - squareSize/2)
                     << "\" width=\"" << squareSize << "\" height=\"" << squareSize
                     << "\" fill=\"none\" stroke=\"#FF4500\" stroke-width=\"3\" rx=\"2\" style=\"filter: drop-shadow(0 0 2px rgba(255,100,0,0.3));\"/>\n";
            }

            file << "<circle cx=\"" << p.x << "\" cy=\"" << p.y
                 << "\" r=\"" << radius << "\" fill=\"" << fillColor
                 << "\" stroke=\"" << strokeColor << "\" stroke-width=\"" << strokeWidth
                 << "\" class=\"" << circleClass << "\" style=\"filter: drop-shadow(0 0 2px rgba(0,0,0,0.3));\"/>\n";

            file << "<text x=\"" << (p.x + 14) << "\" y=\"" << (p.y + 5)
                 << "\" class=\"vertex-label\" font-weight=\"bold\">" << i << "</text>\n";
        }
    }

    void drawGuards(std::ofstream& file, const BoundingBox& bbox) {
        if (!guardSet) return;

        const auto& vertices = polygon.getVertices();
        const auto& guards = guardSet->getGuards();

        file << "<!-- Guard Markers -->\n";
        for (int guardIdx : guards) {
            Point p = transformPoint(vertices[guardIdx], bbox);

            file << "<rect x=\"" << (p.x - 10) << "\" y=\"" << (p.y - 10)
                 << "\" width=\"20\" height=\"20\" fill=\"none\" stroke=\"#FF4500\" "
                 << "stroke-width=\"2\" class=\"guard-marker\"/>\n";
        }
    }
};

#endif
