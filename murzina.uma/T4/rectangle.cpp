#include "rectangle.h"

Rectangle::Rectangle(Point lb, double w, double h)
    : leftBottom_(lb), width_(w), height_(h) {}

double Rectangle::getArea() const
{
    return width_ * height_;
}

Point Rectangle::getCenter() const
{
    return {leftBottom_.x + width_ / 2, leftBottom_.y + height_ / 2};
}

void Rectangle::move(double dx, double dy)
{
    leftBottom_.x += dx;
    leftBottom_.y += dy;
}

void Rectangle::scale(double k)
{
    width_ *= k;
    height_ *= k;
}

std::string Rectangle::getName() const
{
    return "RECTANGLE";
}
