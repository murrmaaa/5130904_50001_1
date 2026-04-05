#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"

class Rectangle : public Shape {
public:
    Rectangle(double x1, double y1, double x2, double y2);

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double factor) override;
    std::string getName() const override;

private:
    double left, bottom, right, top;
};

#endif // RECTANGLE_H

