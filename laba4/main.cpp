#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>
#include "include/triangle.hpp"
#include "include/square.hpp"
#include "include/octagon.hpp"
#include "include/array.hpp"

int main() {
    using T = double;
    using FigurePtr = std::shared_ptr<Figure<T>>;
    Array<FigurePtr> arr;

    auto t = std::make_shared<Triangle<T>>();
    std::istringstream ts("0 0 1 0 0.5 0.866025");
    ts >> *t;

    auto s = std::make_shared<Square<T>>();
    std::istringstream ss("0 0 1 0 1 1 0 1");
    ss >> *s;

    auto o = std::make_shared<Octagon<T>>();
    std::istringstream os(
        "3.931852 1.517638 3.000000 2.732051 1.482362 2.931852 0.267949 2.000000 "
        "0.068148 0.482362 1.000000 -0.732051 2.517638 -0.931852 3.732051 0.000000"
    );
    os >> *o;

    arr.push_back(t);
    arr.push_back(s);
    arr.push_back(o);

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "Список фигур:" << std::endl;
    arr.printAll();
    std::cout << "Общая площадь: " << arr.totalArea() << std::endl;

    std::cout << "\nУдаляю вторую фигуру..." << std::endl;
    arr.removeAt(1);

    std::cout << "После удаления:" << std::endl;
    arr.printAll();
    std::cout << "Общая площадь: " << arr.totalArea() << std::endl;

    auto t_new = std::make_shared<Triangle<T>>();
    std::istringstream tt("1 1 2 1 1.5 1.866025");
    tt >> *t_new;

    arr.push_back(t_new);
    std::cout << "\nПосле добавления новой фигуры:" << std::endl;
    arr.printAll();
    std::cout << "Общая площадь: " << arr.totalArea() << std::endl;

    return 0;
}
