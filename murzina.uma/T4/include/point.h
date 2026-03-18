#pragma once
#include <iostream>
#include <iomanip>

struct Point {
    double x{0}, y{0};

    Point(double x_ = 0, double y_ = 0) : x(x_), y(y_) {}

    Point& operator+=(const Point& p) { x += p.x; y += p.y; return *this; }
    Point& operator-=(const Point& p) { x -= p.x; y -= p.y; return *this; }
    Point& operator*=(double k) { x *= k; y *= k; return *this; }

    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << std::fixed << std::setprecision(2) << "(" << p.x << ", " << p.y << ")";
        return os;
    }
};

inline Point operator+(Point a, const Point& b) { return a += b; }
inline Point operator-(Point a, const Point& b) { return a -= b; }
inline Point operator*(Point a, double k) { return a *= k; }
inline Point operator*(double k, Point a) { return a *= k; }
