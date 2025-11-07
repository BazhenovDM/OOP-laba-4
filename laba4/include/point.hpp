#pragma once
#include "concepts.hpp"
#include <ostream>
#include <istream>
#include <cmath>
#include <type_traits>

template<Scalar T>
class Point {
    T x_;
    T y_;
public:
    Point(): x_(T{}), y_(T{}) {}
    Point(T x, T y): x_(x), y_(y) {}
    Point(const Point& other) = default;
    Point(Point&& other) noexcept = default;
    ~Point() = default;
    Point& operator=(const Point& other) = default;
    Point& operator=(Point&& other) noexcept = default;

    T getX() const { return x_; }
    T getY() const { return y_; }
    void setX(T v) { x_ = v; }
    void setY(T v) { y_ = v; }

    Point operator+(const Point& r) const { return Point(x_ + r.x_, y_ + r.y_); }
    Point operator-(const Point& r) const { return Point(x_ - r.x_, y_ - r.y_); }
    Point& operator+=(const Point& r) { x_ += r.x_; y_ += r.y_; return *this; }
    Point& operator-=(const Point& r) { x_ -= r.x_; y_ -= r.y_; return *this; }
};

template<Scalar T>
std::ostream& operator<<(std::ostream& os, const Point<T>& p) {
    os << "(" << p.getX() << ", " << p.getY() << ")";
    return os;
}

template<Scalar T>
std::istream& operator>>(std::istream& is, Point<T>& p) {
    T x, y;
    is >> x >> y;
    if (is) {
        p.setX(x);
        p.setY(y);
    }
    return is;
}

template<Scalar T>
bool operator==(const Point<T>& a, const Point<T>& b) {
    if constexpr (std::is_floating_point_v<T>) {
        constexpr long double EPS = 1e-6L;
        long double dx = std::fabsl(static_cast<long double>(a.getX()) - static_cast<long double>(b.getX()));
        long double dy = std::fabsl(static_cast<long double>(a.getY()) - static_cast<long double>(b.getY()));
        return dx < EPS && dy < EPS;
    } else {
        return a.getX() == b.getX() && a.getY() == b.getY();
    }
}
