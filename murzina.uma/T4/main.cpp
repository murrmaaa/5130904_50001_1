#include <iostream>
#include <vector>
#include <memory>

#include "rectangle.h"
#include "trapezoid.h"
#include "compositeShape.h"

void print(const Shape& s)
{
    Point c = s.getCenter();
    std::cout << "[" << s.getName() << ", (" << c.x << ", " << c.y << "), " << s.getArea() << "]\n";
}

int main()
{
    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Rectangle>(Point{0, 0}, 4, 3));
    shapes.push_back(std::make_unique<Trapezoid>(Point{0, 0}, 6, 4, 3));

    CompositeShape comp;
    comp.addShape(std::make_unique<Rectangle>(Point{1, 1}, 2, 2));
    comp.addShape(std::make_unique<Trapezoid>(Point{0, 0}, 4, 2, 2));

    shapes.push_back(std::make_unique<CompositeShape>(comp));

    std::cout << "Before scale:\n";
    for (const auto& s : shapes)
        print(*s);

    for (auto& s : shapes)
        s->scale(2);

    std::cout << "\nAfter scale:\n";
    for (const auto& s : shapes)
        print(*s);

    return 0;
}
