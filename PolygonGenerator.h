#ifndef POLYGONGENERATOR_H
#define POLYGONGENERATOR_H

#include "Geometry.h"
#include "Polygon.h"
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>

class PolygonGenerator {
public:
    static std::vector<Point> generateRandomSimplePolygon(int numVertices, double minCoord = -100.0, double maxCoord = 100.0) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> coordDis(minCoord, maxCoord);

        const int MAX_ATTEMPTS = 200;
        for (int attempt = 0; attempt < MAX_ATTEMPTS; attempt++) {
            std::vector<Point> vertices;

            // Generate n independent random points in [minCoord, maxCoord] x [minCoord, maxCoord]
            for (int i = 0; i < numVertices; i++) {
                double x = coordDis(gen);
                double y = coordDis(gen);
                vertices.push_back(Point(x, y));
            }

            // Compute centroid
            double cx = 0.0, cy = 0.0;
            for (const auto& p : vertices) {
                cx += p.x;
                cy += p.y;
            }
            cx /= numVertices;
            cy /= numVertices;

            // Sort points by angle around centroid (creates a simple, non-self-intersecting polygon)
            std::sort(vertices.begin(), vertices.end(), [cx, cy](const Point& a, const Point& b) {
                return std::atan2(a.y - cy, a.x - cx) < std::atan2(b.y - cy, b.x - cx);
            });

            // Validate that it forms a valid simple polygon
            Polygon poly(vertices);
            if (poly.isValid()) {
                return vertices;
            }
        }

        // Fallback: if we exhaust attempts, log a warning and return the last attempt
        std::cerr << "Warning: could not generate valid random simple polygon after " << MAX_ATTEMPTS << " attempts\n";
        std::vector<Point> fallback;
        for (int i = 0; i < numVertices; i++) {
            fallback.push_back(Point(coordDis(gen), coordDis(gen)));
        }
        return fallback;
    }

    static std::vector<Point> generateRandomConvexPolygon(int numVertices, double domainSize = 100.0) {
        std::random_device rd;
        std::mt19937 gen(rd());

        // Generate random angles (sorted) for guaranteed convex polygon
        std::uniform_real_distribution<> angleDis(0.0, 2.0 * M_PI);
        std::vector<double> angles;

        for (int i = 0; i < numVertices; i++) {
            angles.push_back(angleDis(gen));
        }
        std::sort(angles.begin(), angles.end());

        // Generate random radii for each angle
        std::uniform_real_distribution<> radiusDis(domainSize * 0.2, domainSize * 0.35);

        // Center of polygon
        Point center(domainSize / 2.0, domainSize / 2.0);

        std::vector<Point> vertices;
        for (int i = 0; i < numVertices; i++) {
            double radius = radiusDis(gen);
            double x = center.x + radius * std::cos(angles[i]);
            double y = center.y + radius * std::sin(angles[i]);

            // Clamp to domain if needed
            x = std::max(0.0, std::min(domainSize, x));
            y = std::max(0.0, std::min(domainSize, y));

            vertices.push_back(Point(x, y));
        }

        return vertices;
    }

public:

    static std::vector<Point> generateRandomStarPolygon(int numPoints, double outerRadius = 100.0,
                                                        double innerRadius = 40.0) {
        std::vector<Point> vertices;
        int numVertices = numPoints * 2;

        for (int i = 0; i < numVertices; i++) {
            double angle = 2.0 * M_PI * i / numVertices;
            double radius = (i % 2 == 0) ? outerRadius : innerRadius;

            double x = radius * std::cos(angle);
            double y = radius * std::sin(angle);
            vertices.push_back(Point(x, y));
        }

        return vertices;
    }

    static std::vector<Point> generateRectangle(double width, double height) {
        return {
            Point(0, 0),
            Point(width, 0),
            Point(width, height),
            Point(0, height)
        };
    }

    static std::vector<Point> generateTriangle(double size) {
        return {
            Point(0, 0),
            Point(size, 0),
            Point(size / 2.0, size * std::sqrt(3) / 2.0)
        };
    }

    static std::vector<Point> generatePentagon(double radius) {
        std::vector<Point> vertices;
        for (int i = 0; i < 5; i++) {
            double angle = 2.0 * M_PI * i / 5.0;
            double x = radius * std::cos(angle);
            double y = radius * std::sin(angle);
            vertices.push_back(Point(x, y));
        }
        return vertices;
    }

    static std::vector<Point> generateComplexPolygon() {
        return {
            Point(0, 0),
            Point(100, 0),
            Point(150, 50),
            Point(200, 0),
            Point(250, 30),
            Point(200, 100),
            Point(250, 150),
            Point(150, 130),
            Point(100, 180),
            Point(50, 150),
            Point(0, 180),
            Point(-20, 100),
            Point(20, 80),
            Point(10, 50)
        };
    }

    static std::vector<Point> generateHexagon(double radius) {
        std::vector<Point> vertices;
        for (int i = 0; i < 6; i++) {
            double angle = 2.0 * M_PI * i / 6.0;
            double x = radius * std::cos(angle);
            double y = radius * std::sin(angle);
            vertices.push_back(Point(x, y));
        }
        return vertices;
    }

    static std::vector<Point> generateOctagon(double radius) {
        std::vector<Point> vertices;
        for (int i = 0; i < 8; i++) {
            double angle = 2.0 * M_PI * i / 8.0;
            double x = radius * std::cos(angle);
            double y = radius * std::sin(angle);
            vertices.push_back(Point(x, y));
        }
        return vertices;
    }
};

#endif
