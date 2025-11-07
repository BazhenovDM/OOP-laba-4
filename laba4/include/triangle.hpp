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
        long double sx = 0.0L, sy = 0.0L;
        for (std::size_t i = 0; i < 3; ++i) {
            sx += static_cast<long double>(Figure<T>::pointAt(i).getX());
            sy += static_cast<long double>(Figure<T>::pointAt(i).getY());
        }
        return Point<T>(static_cast<T>(sx / 3.0L), static_cast<T>(sy / 3.0L));
    }

    bool isCorrect() const override {
        constexpr long double EPS = 1e-6L;
        long double ds[3];
        for (int i = 0; i < 3; ++i) {
            int j = (i + 1) % 3;
            auto xi = static_cast<long double>(Figure<T>::pointAt(i).getX());
            auto yi = static_cast<long double>(Figure<T>::pointAt(i).getY());
            auto xj = static_cast<long double>(Figure<T>::pointAt(j).getX());
            auto yj = static_cast<long double>(Figure<T>::pointAt(j).getY());
            long double dx = xj - xi;
            long double dy = yj - yi;
            ds[i] = dx*dx + dy*dy;
            if (ds[i] < EPS) return false;
        }
        return (std::fabsl(ds[0] - ds[1]) < EPS) && (std::fabsl(ds[1] - ds[2]) < EPS);
    }

    explicit operator double() const override {
        long double area2 = 0.0L;
        for (std::size_t i = 0; i < 3; ++i) {
            std::size_t j = (i + 1) % 3;
            long double xi = static_cast<long double>(Figure<T>::pointAt(i).getX());
            long double yi = static_cast<long double>(Figure<T>::pointAt(i).getY());
            long double xj = static_cast<long double>(Figure<T>::pointAt(j).getX());
            long double yj = static_cast<long double>(Figure<T>::pointAt(j).getY());
            area2 += xi * yj - xj * yi;
        }
        return static_cast<double>(0.5L * std::fabsl(area2));
    }

    std::unique_ptr<Figure<T>> clone() const override {
        return std::make_unique<Triangle<T>>(*this);
    }
};
