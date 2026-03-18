#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.hpp"

class Rectangle : public Shape {
public:
    Rectangle(Point bottomLeft, Point topRight);
    double getArea() const;
    Point getCenter() const;
    void move(double dx, double dy);
    void scale(double factor);
    std::string getName() const;
private:
    Point bottomLeft_;
    Point topRight_;
};

#endif
