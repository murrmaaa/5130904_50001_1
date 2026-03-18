#pragma once
#include "shape.h"
#include <vector>
#include <memory>

class CompositeShape : public Shape {
public:
    CompositeShape() = default;
    void addShape(std::unique_ptr<Shape> shape);

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double coefficient) override;
    std::string getName() const override { return "COMPOSITE"; }
    std::unique_ptr<Shape> clone() const override;
    void print(std::ostream& os) const override;

private:
    std::vector<std::unique_ptr<Shape>> shapes_;
    struct BBox { double minX, maxX, minY, maxY; };
    BBox getBoundingBox() const;
};
