#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <type_traits>
#include "point.hpp"

template <typename T>
requires std::is_arithmetic_v<T>
class Figure {
protected:
    std::vector<std::unique_ptr<Point<T>>> points;
    std::size_t length;

    void rebuild_points_if_needed() {
        if (points.size() != length) {
            points.clear();
            points.reserve(length);
            for (std::size_t i = 0; i < length; ++i)
                points.push_back(std::make_unique<Point<T>>(Point<T>()));
        }
    }

public:
    Figure(std::size_t len = 0) : points(), length(len) {
        rebuild_points_if_needed();
    }

    virtual ~Figure() = default;

    virtual bool isCorrect() const = 0;
    virtual Point<T> getCenter() const = 0;
    virtual operator double() const = 0;
    virtual std::unique_ptr<Figure<T>> clone() const = 0;

    virtual void input(std::istream& is) {
        rebuild_points_if_needed();
        for (std::size_t i = 0; i < length; ++i) {
            T x, y;
            is >> x >> y;
            points[i]->setX(x);
            points[i]->setY(y);
        }
    }

    virtual void output(std::ostream& os) const {
        for (std::size_t i = 0; i < length; ++i) os << *points[i] << " ";
    }

    bool operator==(const Figure& other) const {
        if (length != other.length) return false;
        for (std::size_t i = 0; i < length; ++i)
            if (!(*points[i] == *other.points[i])) return false;
        return true;
    }

    Figure(const Figure& other) : points(), length(other.length) {
        points.reserve(length);
        for (std::size_t i = 0; i < other.length; ++i)
            points.push_back(std::make_unique<Point<T>>(*other.points[i]));
    }

    Figure(Figure&& other) noexcept : points(std::move(other.points)), length(other.length) {}

    Figure& operator=(const Figure& other) {
        if (this != &other) {
            length = other.length;
            points.clear();
            points.reserve(length);
            for (std::size_t i = 0; i < other.length; ++i)
                points.push_back(std::make_unique<Point<T>>(*other.points[i]));
        }
        return *this;
    }

    Figure& operator=(Figure&& other) noexcept {
        if (this != &other) {
            length = other.length;
            points = std::move(other.points);
        }
        return *this;
    }

    Point<T>& pointAt(std::size_t idx) {
        return *points[idx];
    }

    const Point<T>& pointAt(std::size_t idx) const {
        return *points[idx];
    }

    std::size_t size() const noexcept { return length; }

    friend std::istream& operator>>(std::istream& is, Figure<T>& f) {
        f.input(is);
        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, const Figure<T>& f) {
        f.output(os);
        return os;
    }
};
