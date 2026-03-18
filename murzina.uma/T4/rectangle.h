#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"

class Rectangle : public Shape
{
private:
    Point leftBottom_;
    double width_;
    double height_;

public:
    Rectangle(Point lb, double w, double h);

    double getArea() const override;
    Point getCenter() const override;

    void move(double dx, double dy) override;
    void scale(double k) override;

    std::string getName() const override;
};

#endif
