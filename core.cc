#include <algorithm>
#include <iterator>
#include <exception>
#include <stdexcept>
#include <system_error>

#include <cstdio>
#include <cstdlib>

#ifdef __unix__
#include <errno.h>    // errno
#include <unistd.h>   // POSIX, sysconf
#include <sys/mman.h> // mincore(2)
#endif

#include "mapped_file.h"
#include "file_mapping.h"


int main() try {
  // This uses Boost.Iostream's mapped_file abstraction.
  // Does not support madvise, or anything more than just a simple mmap use-case.
  // auto region = make_mapped_file();

  // This uses Boost.Interprocess' file_mapping abstraction.
  // Supports setting madvise via enum and passing flags to the underlying mmap.
  const auto region = make_file_mapping();

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

} catch (const std::exception& e) {
  std::fprintf(stderr, "Error: %s\n", e.what());
  return EXIT_FAILURE;
}
