#include "CompositeShape.h"
#include <stdexcept>
#include <algorithm>

void CompositeShape::addShape(std::unique_ptr<Shape> shape) {
    if (!shape) {
        throw std::invalid_argument("Cannot add null shape!");
    }
    shapes_.push_back(std::move(shape));
}

double CompositeShape::getArea() const {
    double total = 0.0;
    for (const auto& shape : shapes_) {
        total += shape->getArea();
    }
    return total;
}

Point CompositeShape::getCenter() const {
    if (shapes_.empty()) {
        throw std::logic_error("Composite shape is empty!");
    }

    double minX = shapes_[0]->getCenter().x;
    double minY = shapes_[0]->getCenter().y;
    double maxX = minX;
    double maxY = minY;

    for (const auto& shape : shapes_) {
        Point center = shape->getCenter();
        minX = std::min(minX, center.x);
        minY = std::min(minY, center.y);
        maxX = std::max(maxX, center.x);
        maxY = std::max(maxY, center.y);
    }

    return {(minX + maxX) / 2.0, (minY + maxY) / 2.0};
}

void CompositeShape::move(double dx, double dy) {
    for (auto& shape : shapes_) {
        shape->move(dx, dy);
    }
}

void CompositeShape::scale(double factor) {
    if (factor <= 0) {
        throw std::invalid_argument("Scale factor must be positive!");
    }

    Point compositeCenter = getCenter();

    for (auto& shape : shapes_) {
        Point shapeCenter = shape->getCenter();

        double dx = shapeCenter.x - compositeCenter.x;
        double dy = shapeCenter.y - compositeCenter.y;

        shape->move(dx * (factor - 1.0), dy * (factor - 1.0));
        shape->scale(factor);
    }
}

std::string CompositeShape::getName() const {
    return "COMPOSITE";
}

const std::vector<std::unique_ptr<Shape>>& CompositeShape::getShapes() const {
    return shapes_;
}
