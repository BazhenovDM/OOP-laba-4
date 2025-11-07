#pragma once
#include "concepts.hpp"
#include <ostream>
#include <istream>
#include <cmath>
#include <type_traits>

template<Scalar T>
class Point {
    T x;
    T y;
public:
    Point(): x(T{}), y(T{}) {}
    Point(T x_, T y_): x(x_), y(y_) {}
    Point(const Point& other) = default;
    Point(Point&& other) noexcept = default;
    ~Point() = default;
    Point& operator=(const Point& other) = default;
    Point& operator=(Point&& other) noexcept = default;

    T getX() const { return x; }
    T getY() const { return y; }
    void setX(T v) { x = v; }
    void setY(T v) { y = v; }

    Point operator+(const Point& r) const { return Point(x + r.x, y + r.y); }
    Point operator-(const Point& r) const { return Point(x - r.x, y - r.y); }
    Point& operator+=(const Point& r) { x += r.x; y += r.y; return *this; }
    Point& operator-=(const Point& r) { x -= r.x; y -= r.y; return *this; }
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
    if constexpr(std::is_floating_point_v<T>) {
        long double EPS = 1e-6L;
        return std::fabsl(static_cast<long double>(a.getX()) - static_cast<long double>(b.getX())) < EPS
            && std::fabsl(static_cast<long double>(a.getY()) - static_cast<long double>(b.getY())) < EPS;
    } else {
        return a.getX() == b.getX() && a.getY() == b.getY();
    }
}
