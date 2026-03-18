#pragma once
#include "point.h"
#include <memory>
#include <string>
#include <iostream>
#include <iomanip>

class Shape {
public:
    virtual ~Shape() = default;

    virtual double getArea() const = 0;
    virtual Point getCenter() const = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void scale(double coefficient) = 0;
    virtual std::string getName() const = 0;

    virtual void print(std::ostream& os) const {
        os << "[" << getName() << "," << getCenter() << ", "
           << std::fixed << std::setprecision(2) << getArea() << "]";
    }

    virtual std::unique_ptr<Shape> clone() const = 0;
};

inline std::ostream& operator<<(std::ostream& os, const Shape& s) {
    s.print(os);
    return os;
}
