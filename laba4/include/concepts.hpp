#pragma once
#include <type_traits>

template<typename T>
concept Scalar = std::is_arithmetic_v<T>;
