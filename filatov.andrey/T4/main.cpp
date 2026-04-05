#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include "rectangle.h"
#include "circle.h"
#include "composite_shape.h"

void printShape(const Shape& shape) {
    Point center = shape.getCenter();
    std::cout << std::fixed << std::setprecision(2)
              << "[" << shape.getName() << ", ("
              << center.x << ", " << center.y << "), "
              << shape.getArea() << "]" << std::endl;
}

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;

    // Прямоугольник
    shapes.push_back(std::make_unique<Rectangle>(0, 0, 4, 3));
    // Круг
    shapes.push_back(std::make_unique<Circle>(Point{2, 2}, 1.5));
    // Составная фигура
    auto composite = std::make_unique<CompositeShape>();
    composite->addShape(std::make_unique<Rectangle>(1, 1, 3, 2));
    composite->addShape(std::make_unique<Circle>(Point{2, 2}, 1.0));
    shapes.push_back(std::move(composite));

    std::cout << "Before scaling:\n";
    for (const auto& shape : shapes) {
        printShape(*shape);
    }

    // Масштабирую всё в 2 раза
    for (auto& shape : shapes) {
        shape->scale(2.0);
    }

    std::cout << "\nAfter scaling (factor=2):\n";
    for (const auto& shape : shapes) {
        printShape(*shape);
    }

    return 0;
}

