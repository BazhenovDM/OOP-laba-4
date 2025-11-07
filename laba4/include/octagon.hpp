#pragma once
#include "figure.hpp"
#include <cmath>

template<typename T>
requires std::is_arithmetic_v<T>
class Octagon : public Figure<T> {
public:
    Octagon(): Figure<T>(8) {}
    Octagon(const Octagon& other) = default;
    Octagon(Octagon&& other) noexcept = default;
    Octagon& operator=(const Octagon& other) = default;
    Octagon& operator=(Octagon&& other) noexcept = default;
    ~Octagon() override = default;

    Point<T> getCenter() const override {
        constexpr long double EPS = 1e-9L;
        long double crossSum = 0.0L;
        for (std::size_t i = 0; i < 8; ++i) {
            std::size_t j = (i + 1) % 8;
            long double xi = static_cast<long double>(Figure<T>::pointAt(i).getX());
            long double yi = static_cast<long double>(Figure<T>::pointAt(i).getY());
            long double xj = static_cast<long double>(Figure<T>::pointAt(j).getX());
            long double yj = static_cast<long double>(Figure<T>::pointAt(j).getY());
            crossSum += xi * yj - xj * yi;
        }
        long double area2 = 0.5L * crossSum;
        if (std::fabsl(area2) < EPS) return Point<T>(T{}, T{});

        long double cx = 0.0L, cy = 0.0L;
        for (std::size_t i = 0; i < 8; ++i) {
            std::size_t j = (i + 1) % 8;
            long double xi = static_cast<long double>(Figure<T>::pointAt(i).getX());
            long double yi = static_cast<long double>(Figure<T>::pointAt(i).getY());
            long double xj = static_cast<long double>(Figure<T>::pointAt(j).getX());
            long double yj = static_cast<long double>(Figure<T>::pointAt(j).getY());
            long double cross = xi * yj - xj * yi;
            cx += (xi + xj) * cross;
            cy += (yi + yj) * cross;
        }
        cx /= (6.0L * area2);
        cy /= (6.0L * area2);
        return Point<T>(static_cast<T>(cx), static_cast<T>(cy));
    }

    bool isCorrect() const override {
        constexpr long double EPS = 1e-6L;
        if (Figure<T>::length != 8) return false;
        long double sides[8];
        for (int i = 0; i < 8; ++i) {
            int j = (i + 1) % 8;
            long double xi = static_cast<long double>(Figure<T>::pointAt(i).getX());
            long double yi = static_cast<long double>(Figure<T>::pointAt(i).getY());
            long double xj = static_cast<long double>(Figure<T>::pointAt(j).getX());
            long double yj = static_cast<long double>(Figure<T>::pointAt(j).getY());
            long double dx = xj - xi;
            long double dy = yj - yi;
            sides[i] = dx*dx + dy*dy;
            if (sides[i] < EPS) return false;
        }
        for (int i = 1; i < 8; ++i) {
            if (std::fabsl(sides[i] - sides[0]) > EPS) return false;
        }
        double area = static_cast<double>(static_cast<const Octagon&>(*this));
        if (area < 1e-9) return false;
        return true;
    }

    explicit operator double() const override {
        long double acc = 0.0L;
        for (std::size_t i = 0; i < 8; ++i) {
            std::size_t j = (i + 1) % 8;
            long double xi = static_cast<long double>(Figure<T>::pointAt(i).getX());
            long double yi = static_cast<long double>(Figure<T>::pointAt(i).getY());
            long double xj = static_cast<long double>(Figure<T>::pointAt(j).getX());
            long double yj = static_cast<long double>(Figure<T>::pointAt(j).getY());
            acc += xi * yj - xj * yi;
        }
        return static_cast<double>(0.5L * std::fabsl(acc));
    }

    std::unique_ptr<Figure<T>> clone() const override {
        return std::make_unique<Octagon<T>>(*this);
    }
};
