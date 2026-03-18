#ifndef COMPOSITE_SHAPE_H
#define COMPOSITE_SHAPE_H

#include "Shape.hpp"

class CompositeShape : public Shape {
public:
    CompositeShape();
    ~CompositeShape();
    void addShape(Shape* shape);
    int getCount() const;
    Shape* getShape(int index) const;
    double getArea() const;
    Point getCenter() const;
    void move(double dx, double dy);
    void scale(double factor);
    std::string getName() const;
private:
    Shape** shapes_;
    int count_;
};

#endif
