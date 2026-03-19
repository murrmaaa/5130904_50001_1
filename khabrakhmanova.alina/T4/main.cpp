#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <cstdlib>
#include "rectangle.h"
#include "circle.h"
#include "CompositeShape.h"

void printShape(const Shape& shape) {
    Point center = shape.getCenter();
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "[" << shape.getName() << ", ("
              << center.x << ", " << center.y << "), "
              << shape.getArea() << "]";

    const CompositeShape* composite = dynamic_cast<const CompositeShape*>(&shape);
    if (composite) {
        std::cout << ":";
        for (const auto& subShape : composite->getShapes()) {
            std::cout << "\n  ";
            printShape(*subShape);
        }
    }
    std::cout << "\n";
}

int main() {
    try {
        auto rect1 = std::make_unique<Rectangle>(Point{0, 0}, Point{4, 3});
        auto rect2 = std::make_unique<Rectangle>(Point{1, 1}, Point{5, 4});
        auto circle1 = std::make_unique<Circle>(Point{2, 2}, 2.0);
        auto circle2 = std::make_unique<Circle>(Point{6, 6}, 1.5);
        auto rect3 = std::make_unique<Rectangle>(Point{-2, -2}, Point{0, 0});

        auto composite = std::make_unique<CompositeShape>();
        composite->addShape(std::make_unique<Rectangle>(*rect1));
        composite->addShape(std::make_unique<Circle>(*circle1));
        composite->addShape(std::make_unique<Rectangle>(*rect3));

        std::vector<std::unique_ptr<Shape>> shapes;
        shapes.push_back(std::move(rect1));
        shapes.push_back(std::move(rect2));
        shapes.push_back(std::move(circle1));
        shapes.push_back(std::move(circle2));
        shapes.push_back(std::move(rect3));
        shapes.push_back(std::move(composite));

        std::cout << "Before scaling:\n";
        for (const auto& shape : shapes) {
            printShape(*shape);
        }

        for (auto& shape : shapes) {
            shape->scale(2.0);
        }

        std::cout << "\nAfter scaling:\n";
        for (const auto& shape : shapes) {
            printShape(*shape);
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
