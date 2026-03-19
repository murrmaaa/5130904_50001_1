#include "compositeShape.h"

void CompositeShape::addShape(std::unique_ptr<Shape> shape)
{
    shapes_.push_back(std::move(shape));
}

double CompositeShape::getArea() const
{
    double sum = 0;
    for (const auto& s : shapes_)
        sum += s->getArea();
    return sum;
}

Point CompositeShape::getCenter() const
{
    double totalArea = getArea();
    double x = 0, y = 0;

    for (const auto& s : shapes_)
    {
        Point c = s->getCenter();
        double area = s->getArea();
        x += c.x * area;
        y += c.y * area;
    }

    return {x / totalArea, y / totalArea};
}

void CompositeShape::move(double dx, double dy)
{
    for (auto& s : shapes_)
        s->move(dx, dy);
}

void CompositeShape::scale(double k)
{
    for (auto& s : shapes_)
        s->scale(k);
}

std::string CompositeShape::getName() const
{
    return "COMPOSITE";
}
