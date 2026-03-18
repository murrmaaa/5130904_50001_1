#include "include/rectangle.h"
#include <algorithm>

Rectangle::Rectangle(Point bottomLeft, Point topRight)
    : bl_(bottomLeft), tr_(topRight) {
    if (bl_.x > tr_.x) std::swap(bl_.x, tr_.x);
    if (bl_.y > tr_.y) std::swap(bl_.y, tr_.y);
}

double Rectangle::getArea() const { return width() * height(); }

Point Rectangle::getCenter() const {
    return Point((bl_.x + tr_.x) / 2.0, (bl_.y + tr_.y) / 2.0);
}

void Rectangle::move(double dx, double dy) {
    bl_.x += dx; bl_.y += dy;
    tr_.x += dx; tr_.y += dy;
}

void Rectangle::scale(double k) {
    if (k <= 0) return;
    Point c = getCenter();
    bl_ = c + (bl_ - c) * k;
    tr_ = c + (tr_ - c) * k;
}

std::unique_ptr<Shape> Rectangle::clone() const {
    return std::make_unique<Rectangle>(*this);
}
