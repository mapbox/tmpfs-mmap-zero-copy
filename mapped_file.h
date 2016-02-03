#pragma once

#include <boost/iostreams/device/mapped_file.hpp>

#include <vector>
#include <iterator>

#include <cstdio>

#include "jujd.h"


inline auto make_mapped_file(const char* path) {
  using namespace boost::iostreams;

  mapped_file mapping{path, mapped_file::readonly};

  std::printf("open: %ju\n", ju(mapping.is_open()));
  std::printf("size: %ju\n", ju(mapping.size()));
  std::printf("alignment: %jd\n", jd(mapping.alignment()));
  std::printf("address: %p\n", mapping.const_data());

  std::vector<std::uint8_t> bytes(std::begin(mapping), std::end(mapping));
  std::printf("bytes: %ju\n", ju(bytes.size()));

  return mapping;
}
