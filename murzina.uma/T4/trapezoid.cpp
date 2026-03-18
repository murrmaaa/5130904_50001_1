#include "trapezoid.h"

Trapezoid::Trapezoid(Point lb, double a, double b, double h)
    : leftBottom_(lb), bottomBase_(a), topBase_(b), height_(h) {}

double Trapezoid::getArea() const
{
    return (bottomBase_ + topBase_) / 2 * height_;
}

Point Trapezoid::getCenter() const
{
    return {
        leftBottom_.x + (bottomBase_ + topBase_) / 4,
        leftBottom_.y + height_ / 2
    };
}

void Trapezoid::move(double dx, double dy)
{
    leftBottom_.x += dx;
    leftBottom_.y += dy;
}

void Trapezoid::scale(double k)
{
    bottomBase_ *= k;
    topBase_ *= k;
    height_ *= k;
}

std::string Trapezoid::getName() const
{
    return "TRAPEZOID";
}
