#pragma once
#include "figure.hpp"
#include <cmath>

template<typename T>
requires std::is_arithmetic_v<T>
class Square : public Figure<T> {
public:
    Square(): Figure<T>(4) {}
    Square(const Square& other) = default;
    Square(Square&& other) noexcept = default;
    Square& operator=(const Square& other) = default;
    Square& operator=(Square&& other) noexcept = default;
    ~Square() override = default;

    Point<T> getCenter() const override {
        long double cx = 0.0L;
        long double cy = 0.0L;
        for (std::size_t i = 0; i < 4; ++i) {
            cx += static_cast<long double>(Figure<T>::pointAt(i).getX());
            cy += static_cast<long double>(Figure<T>::pointAt(i).getY());
        }
        return Point<T>(static_cast<T>(cx / 4.0L), static_cast<T>(cy / 4.0L));
    }

    bool isCorrect() const override {
        const long double EPS = 1e-6L;
        long double d[4];
        for (int i = 0; i < 4; ++i) {
            int j = (i + 1) % 4;
            long double dx = static_cast<long double>(Figure<T>::pointAt(j).getX()) - static_cast<long double>(Figure<T>::pointAt(i).getX());
            long double dy = static_cast<long double>(Figure<T>::pointAt(j).getY()) - static_cast<long double>(Figure<T>::pointAt(i).getY());
            d[i] = dx*dx + dy*dy;
            if (d[i] < EPS) return false;
        }
        for (int i = 1; i < 4; ++i) {
            if (std::fabsl(d[i] - d[0]) > EPS) return false;
        }
        long double dx1 = static_cast<long double>(Figure<T>::pointAt(1).getX()) - static_cast<long double>(Figure<T>::pointAt(0).getX());
        long double dy1 = static_cast<long double>(Figure<T>::pointAt(1).getY()) - static_cast<long double>(Figure<T>::pointAt(0).getY());
        long double dx2 = static_cast<long double>(Figure<T>::pointAt(2).getX()) - static_cast<long double>(Figure<T>::pointAt(1).getX());
        long double dy2 = static_cast<long double>(Figure<T>::pointAt(2).getY()) - static_cast<long double>(Figure<T>::pointAt(1).getY());
        long double dot = dx1*dx2 + dy1*dy2;
        return std::fabsl(dot) < EPS;
    }

    explicit operator double() const override {
        long double s = 0.0L;
        for (std::size_t i = 0; i < 4; ++i) {
            std::size_t j = (i + 1) % 4;
            long double xi = static_cast<long double>(Figure<T>::pointAt(i).getX());
            long double yi = static_cast<long double>(Figure<T>::pointAt(i).getY());
            long double xj = static_cast<long double>(Figure<T>::pointAt(j).getX());
            long double yj = static_cast<long double>(Figure<T>::pointAt(j).getY());
            s += xi * yj - xj * yi;
        }
        return static_cast<double>(0.5L * std::fabsl(s));
    }

    std::unique_ptr<Figure<T>> clone() const override {
        return std::make_unique<Square<T>>(*this);
    }
};
