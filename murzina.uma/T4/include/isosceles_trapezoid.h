#pragma once
#include "shape.h"

class IsoscelesTrapezoid : public Shape {
public:
    IsoscelesTrapezoid(Point bottomLeft, double bottomBase, double topBase, double height);

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double coefficient) override;
    std::string getName() const override { return "TRAPEZOID_ISOSCELES"; }
    std::unique_ptr<Shape> clone() const override;

private:
    Point bl_;
    double bottomBase_, topBase_, height_;
    double topOffset() const { return (bottomBase_ - topBase_) / 2.0; }
};
