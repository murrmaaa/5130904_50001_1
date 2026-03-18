#pragma once
#include "shape.h"

class Rectangle : public Shape {
public:
    Rectangle(Point bottomLeft, Point topRight);

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double coefficient) override;
    std::string getName() const override { return "RECTANGLE"; }
    std::unique_ptr<Shape> clone() const override;

private:
    Point bl_, tr_;
    double width() const { return tr_.x - bl_.x; }
    double height() const { return tr_.y - bl_.y; }
};
