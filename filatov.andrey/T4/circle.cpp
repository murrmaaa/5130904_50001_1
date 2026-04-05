#include "circle.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <stdexcept>

Circle::Circle(const Point& c, double r) : center(c), radius(r) {
    if (radius <= 0) {
        throw std::invalid_argument("Radius must be positive");
    }
}

double Circle::getArea() const {
    return M_PI * radius * radius;
}

Point Circle::getCenter() const {
    return center;
}

void Circle::move(double dx, double dy) {
    center.x += dx;
    center.y += dy;
}

void Circle::scale(double factor) {
    if (factor <= 0) {
        throw std::invalid_argument("Scale factor must be positive");
    }
    radius *= factor;
}

std::string Circle::getName() const {
    return "CIRCLE";
}

