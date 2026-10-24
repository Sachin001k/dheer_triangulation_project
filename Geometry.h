#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>
#include <vector>
#include <iostream>

struct Point {
    double x, y;

    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}

    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
    Point operator*(double t) const { return Point(x * t, y * t); }

    double dot(const Point& p) const { return x * p.x + y * p.y; }
    double cross(const Point& p) const { return x * p.y - y * p.x; }
    double distance(const Point& p) const {
        double dx = x - p.x, dy = y - p.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    bool operator==(const Point& p) const {
        const double EPS = 1e-9;
        return std::abs(x - p.x) < EPS && std::abs(y - p.y) < EPS;
    }
};

struct Segment {
    Point a, b;

    Segment() {}
    Segment(const Point& a, const Point& b) : a(a), b(b) {}

    Point direction() const { return b - a; }
    double length() const { return a.distance(b); }
};

class GeometryUtils {
public:
    static constexpr double EPS = 1e-9;

    static double ccw(const Point& a, const Point& b, const Point& c) {
        return (b - a).cross(c - a);
    }

    static bool isConvex(const Point& a, const Point& b, const Point& c) {
        return ccw(a, b, c) > -EPS;
    }

    static bool pointInTriangle(const Point& p, const Point& a, const Point& b, const Point& c) {
        double d1 = ccw(p, a, b);
        double d2 = ccw(p, b, c);
        double d3 = ccw(p, c, a);

        bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
        bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

        return !(hasNeg && hasPos);
    }

    static bool segmentsIntersect(const Point& p1, const Point& p2,
                                   const Point& p3, const Point& p4) {
        double d1 = ccw(p3, p4, p1);
        double d2 = ccw(p3, p4, p2);
        double d3 = ccw(p1, p2, p3);
        double d4 = ccw(p1, p2, p4);

        if (((d1 > EPS && d2 < -EPS) || (d1 < -EPS && d2 > EPS)) &&
            ((d3 > EPS && d4 < -EPS) || (d3 < -EPS && d4 > EPS))) {
            return true;
        }
        return false;
    }

    static double polygonSignedArea(const std::vector<Point>& vertices) {
        double area = 0.0;
        int n = vertices.size();
        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            area += vertices[i].x * vertices[j].y;
            area -= vertices[j].x * vertices[i].y;
        }
        return area / 2.0;
    }
};

#endif
