#pragma once

#include <cstddef>

#include <algorithm>
#include <random>

#include "blob.h"
#include "optimizer.h"


inline void access_seq(const Blob* first, const Blob* last) {
  auto x = std::accumulate(first, last, 0, [](std::size_t sum, Blob b) { return sum + (b.a + b.b); });

  escape(&x);
}


inline void access_rnd(const Blob* first, const Blob* last) {
  const std::size_t distance = last - first;

  static std::mt19937 mt{0xCAFE};
  std::uniform_int_distribution<std::size_t> dist{0u, distance};

  std::size_t count{0};

  for (std::size_t i{0}; i < distance / 4u; ++i) {
    auto at = dist(mt);
    count += first[at].a + first[at].b;
  }

  escape(&count);
}
