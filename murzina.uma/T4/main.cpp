#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include "include/shape.h"
#include "include/rectangle.h"
#include "include/isosceles_trapezoid.h"
#include "include/composite_shape.h"

int main() {
    std::cout << std::fixed << std::setprecision(2);

    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Rectangle>(Point(0, 0), Point(4, 3)));
    shapes.push_back(std::make_unique<Rectangle>(Point(10, 5), Point(15, 9)));
    shapes.push_back(std::make_unique<IsoscelesTrapezoid>(Point(2, 10), 8, 4, 3));

    auto comp = std::make_unique<CompositeShape>();
    comp->addShape(std::make_unique<Rectangle>(Point(0, 0), Point(2, 2)));
    comp->addShape(std::make_unique<IsoscelesTrapezoid>(Point(3, 0), 4, 2, 2));
    shapes.push_back(std::move(comp));

    shapes.push_back(std::make_unique<Rectangle>(Point(-5, -5), Point(-2, -1)));

    std::cout << "BEFORE SCALE\n";
    for (const auto& s : shapes) std::cout << *s << "\n";

    for (auto& s : shapes) s->scale(2.0);

    std::cout << "AFTER SCALE x2\n";
    for (const auto& s : shapes) std::cout << *s << "\n";

    return 0;
}
