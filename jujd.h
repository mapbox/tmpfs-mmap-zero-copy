#pragma once

#include <type_traits>

#include <cstdint>


template <typename T>
inline auto jd(T x) {
  static_assert(std::is_signed<T>() and std::is_integral<T>(), "signed integral");
  return static_cast<std::intmax_t>(x);
}

template <typename T>
inline auto ju(T x) {
  static_assert(std::is_unsigned<T>() and std::is_integral<T>(), "unsigned integral");
  return static_cast<std::uintmax_t>(x);
}
