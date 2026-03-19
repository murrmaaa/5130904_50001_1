#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include "shape.h"

class Trapezoid : public Shape
{
private:
    Point leftBottom_;
    double bottomBase_;
    double topBase_;
    double height_;

public:
    Trapezoid(Point lb, double a, double b, double h);

    double getArea() const override;
    Point getCenter() const override;

    void move(double dx, double dy) override;
    void scale(double k) override;

    std::string getName() const override;
};

#endif
