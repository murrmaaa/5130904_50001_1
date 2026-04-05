#include "rectangle.h"
#include <algorithm>
#include <stdexcept>

Rectangle::Rectangle(double x1, double y1, double x2, double y2) {
    if (x1 >= x2 || y1 >= y2) {
        throw std::invalid_argument("Invalid rectangle coordinates");
    }
    left = x1;
    bottom = y1;
    right = x2;
    top = y2;
}

double Rectangle::getArea() const {
    double width = right - left;
    double height = top - bottom;
    return width * height;
}

Point Rectangle::getCenter() const {
    return { (left + right) / 2.0, (bottom + top) / 2.0 };
}

void Rectangle::move(double dx, double dy) {
    left += dx;
    right += dx;
    bottom += dy;
    top += dy;
}

void Rectangle::scale(double factor) {
    if (factor <= 0) {
        throw std::invalid_argument("Scale factor must be positive");
    }
    Point center = getCenter();
    double halfWidth = (right - left) / 2.0 * factor;
    double halfHeight = (top - bottom) / 2.0 * factor;
    left = center.x - halfWidth;
    right = center.x + halfWidth;
    bottom = center.y - halfHeight;
    top = center.y + halfHeight;
}

std::string Rectangle::getName() const {
    return "RECTANGLE";
}

