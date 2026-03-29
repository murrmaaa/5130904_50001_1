#include "composite_shape.h"
#include <algorithm>
#include <stdexcept>
#include <limits>

void CompositeShape::addShape(std::unique_ptr<Shape> shape) {
    if (!shape) {
        throw std::invalid_argument("Cannot add null shape");
    }
    shapes.push_back(std::move(shape));
}

double CompositeShape::getArea() const {
    double total = 0.0;
    for (const auto& s : shapes) {
        total += s->getArea();
    }
    return total;
}

Point CompositeShape::getCenter() const {
    if (shapes.empty()) {
        return {0.0, 0.0};
    }
    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double maxX = -std::numeric_limits<double>::max();
    double maxY = -std::numeric_limits<double>::max();

    for (const auto& s : shapes) {
        Point c = s->getCenter();
        minX = std::min(minX, c.x);
        minY = std::min(minY, c.y);
        maxX = std::max(maxX, c.x);
        maxY = std::max(maxY, c.y);
    }
    return { (minX + maxX) / 2.0, (minY + maxY) / 2.0 };
}

void CompositeShape::move(double dx, double dy) {
    for (auto& s : shapes) {
        s->move(dx, dy);
    }
}

void CompositeShape::scale(double factor) {
    if (factor <= 0) {
        throw std::invalid_argument("Scale factor must be positive");
    }
    Point center = getCenter();
    for (auto& s : shapes) {
   GNU nano 8.7                  composite_shape.cpp                   Modified
        Point oldCenter = s->getCenter();
        double dx = oldCenter.x - center.x;
        double dy = oldCenter.y - center.y;
        dx *= factor;
        dy *= factor;
        s->scale(factor);
        Point newCenter = s->getCenter();
        double deltaX = (center.x + dx) - newCenter.x;
        double deltaY = (center.y + dy) - newCenter.y;
        s->move(deltaX, deltaY);
    }
}

std::string CompositeShape::getName() const {
    return "COMPOSITE";
}


       Point oldCenter = s->getCenter();
        double dx = oldCenter.x - center.x;
        double dy = oldCenter.y - center.y;
        dx *= factor;
        dy *= factor;
        s->scale(factor);
        Point newCenter = s->getCenter();
        double deltaX = (center.x + dx) - newCenter.x;
        double deltaY = (center.y + dy) - newCenter.y;
        s->move(deltaX, deltaY);
    }
}

std::string CompositeShape::getName() const {
    return "COMPOSITE";
}
