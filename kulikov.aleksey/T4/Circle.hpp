#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.hpp"

class Circle : public Shape {
public:
    Circle(Point center, double radius);
    double getArea() const;
    Point getCenter() const;
    void move(double dx, double dy);
    void scale(double factor);
    std::string getName() const;
private:
    Point center_;
    double radius_;
};

#endif
