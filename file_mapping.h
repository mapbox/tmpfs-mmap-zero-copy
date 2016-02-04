#pragma once

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <sys/mman.h>


#include <cstdio>

#ifdef __unix__
#include <unistd.h>   // POSIX
#include <sys/mman.h> // MAP_PRIVATE, MAP_POPULATE
#endif

#include "jujd.h"


inline auto make_file_mapping(const char* path) {
  using namespace boost::interprocess;

  file_mapping mapping{path, read_only};

#ifdef __unix__
  mapped_region region{mapping, read_only, 0, 0, nullptr, MAP_POPULATE};
  //mapped_region region{mapping, read_only};
#else
  mapped_region region{mapping, read_only};
#endif
  //region.advise(mapped_region::advice_willneed);
  //
 auto ret = mlock(region.get_address(), region.get_size());
 // std::printf("mlock: %s\n", ret < 0 ? "failed" : "ok");

 // std::printf("name: %s\n", mapping.get_name());
 // std::printf("size: %ju\n", ju(region.get_size()));
 // std::printf("page size: %ju\n", ju(region.get_page_size()));
 // std::printf("address: %p\n", region.get_address());

  return region;
}
