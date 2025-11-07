#pragma once
#include "figure.hpp"
#include <cmath>

template<typename T>
requires std::is_arithmetic_v<T>
class Triangle : public Figure<T> {
public:
    Triangle(): Figure<T>(3) {}
    Triangle(const Triangle& other) = default;
    Triangle(Triangle&& other) noexcept = default;
    Triangle& operator=(const Triangle& other) = default;
    Triangle& operator=(Triangle&& other) noexcept = default;
    ~Triangle() override = default;

    Point<T> getCenter() const override {
        long double cx = 0.0L;
        long double cy = 0.0L;
        for (std::size_t i = 0; i < 3; ++i) {
            cx += static_cast<long double>(Figure<T>::pointAt(i).getX());
            cy += static_cast<long double>(Figure<T>::pointAt(i).getY());
        }
        return Point<T>(static_cast<T>(cx / 3.0L), static_cast<T>(cy / 3.0L));
    }

    bool isCorrect() const override {
        const long double EPS = 1e-6L;
        long double d[3];
        for (int i = 0; i < 3; ++i) {
            int j = (i + 1) % 3;
            long double dx = static_cast<long double>(Figure<T>::pointAt(j).getX()) - static_cast<long double>(Figure<T>::pointAt(i).getX());
            long double dy = static_cast<long double>(Figure<T>::pointAt(j).getY()) - static_cast<long double>(Figure<T>::pointAt(i).getY());
            d[i] = dx*dx + dy*dy;
            if (d[i] < EPS) return false;
        }
        return std::fabsl(d[0] - d[1]) < EPS && std::fabsl(d[1] - d[2]) < EPS;
    }

    explicit operator double() const override {
        long double s = 0.0L;
        for (std::size_t i = 0; i < 3; ++i) {
            std::size_t j = (i + 1) % 3;
            long double xi = static_cast<long double>(Figure<T>::pointAt(i).getX());
            long double yi = static_cast<long double>(Figure<T>::pointAt(i).getY());
            long double xj = static_cast<long double>(Figure<T>::pointAt(j).getX());
            long double yj = static_cast<long double>(Figure<T>::pointAt(j).getY());
            s += xi * yj - xj * yi;
        }
        return static_cast<double>(0.5L * std::fabsl(s));
    }

    std::unique_ptr<Figure<T>> clone() const override {
        return std::make_unique<Triangle<T>>(*this);
    }
};
