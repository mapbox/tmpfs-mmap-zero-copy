#pragma once

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <system_error>

#include <cstdio>
#include <cerrno>

#ifdef __unix__
#include <unistd.h>   // POSIX, sysconf
#include <sys/mman.h> // mincore(2)
#endif

#include "jujd.h"


template <typename Region>
inline void dump_mincore(const Region& region) {
#ifdef __unix__
  const auto page_size = ::sysconf(_SC_PAGESIZE);
  if (page_size == -1)
    throw std::system_error{errno, std::system_category()};

  std::vector<unsigned char> core((region.get_size() + page_size - 1) / page_size);

  const auto rv = ::mincore(region.get_address(), region.get_size(), core.data());
  if (rv == -1)
    throw std::system_error{errno, std::system_category()};

  const std::size_t in_core = std::count_if(begin(core), end(core), [](const auto byte) { return byte & 1; });

  std::printf("pages: %ju\n", ju(core.size()));
  std::printf("in core: %ju\n", ju(in_core));
#endif
}
