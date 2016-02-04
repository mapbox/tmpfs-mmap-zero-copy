#pragma once

#include <cstddef>

#include <algorithm>
#include <random>

#include "optimizer.h"


inline void access_seq(const Blob* first, const Blob* last) {
  auto x = std::count_if(first, last, [](Blob b) { return b.a == 42 and b.b == 42; });

  escape(&x);
}


inline void access_rnd(const Blob* first, const Blob* last) {
  const std::size_t distance = last - first;

  static std::mt19937 mt{0xCAFE};
  std::uniform_int_distribution<std::size_t> dist{0u, distance};

  std::size_t count{0};

  for (std::size_t i{0}; i < distance / 4u; ++i) {
    auto at = dist(mt);

    if (first[at].a == 42 and first[at].b == 42)
      ++count;
  }

  escape(&count);
}
