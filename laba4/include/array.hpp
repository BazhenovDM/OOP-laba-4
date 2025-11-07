#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <iomanip>
#include <stdexcept>

template<typename E>
class Array {
    std::vector<E> data;
public:
    Array() = default;
    ~Array() = default;
    Array(const Array&) = delete;
    Array& operator=(const Array&) = delete;

    void push_back(E e) {
        if (!e) return;
        data.push_back(std::move(e));
    }

    E at(std::size_t idx) const {
        if (idx >= data.size()) throw std::out_of_range("index");
        return data[idx];
    }

    E operator[](std::size_t idx) const noexcept {
        return data[idx];
    }

    std::size_t size() const noexcept {
        return data.size();
    }

    bool removeAt(std::size_t idx) {
        if (idx >= data.size()) return false;
        data.erase(data.begin() + static_cast<std::ptrdiff_t>(idx));
        return true;
    }

    void clear() noexcept {
        data.clear();
    }

    double totalArea() const noexcept {
        double sum = 0.0;
        for (auto const& e : data) {
            if (!e) continue;
            try {
                if (e->isCorrect()) sum += static_cast<double>(*e);
            } catch (...) {}
        }
        return sum;
    }

    void printAll() const {
        std::cout << std::fixed << std::setprecision(6);
        if (data.empty()) {
            std::cout << "List empty\n";
            return;
        }
        for (std::size_t i = 0; i < data.size(); ++i) {
            auto const& f = data[i];
            if (!f) {
                std::cout << i << ": <null>\n";
                continue;
            }
            std::cout << i << ": " << *f;
            if (f->isCorrect()) {
                double area = 0.0;
                try { area = static_cast<double>(*f); } catch(...) { area = 0.0; }
                auto c = f->getCenter();
                std::cout << " | S=" << area << " | C=" << c << "\n";
            } else {
                std::cout << " | INVALID\n";
            }
        }
    }
};
