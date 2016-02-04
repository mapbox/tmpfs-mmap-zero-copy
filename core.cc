#include <exception>

#include <cstdlib>

#include <boost/timer/timer.hpp>

#include "mapped_file.h"
#include "file_mapping.h"
#include "mincore.h"
#include "blob.h"
#include "access.h"


int main(int argc, char** argv) try {
  if (argc != 2)
    return EXIT_FAILURE;

  // dump_to_binary(argv[1]);

  // This uses Boost.Iostream's mapped_file abstraction.
  // Does not support madvise, or anything more than just a simple mmap use-case.
  // const auto region = make_mapped_file(argv[1]);

  // This uses Boost.Interprocess' file_mapping abstraction.
  // Supports setting madvise via enum and passing flags to the underlying mmap.
  // const auto region = make_file_mapping(argv[1]);

  // dump_mincore(region);

  // 1/ read from file into heap
  const auto vec = read_from_binary(argv[1]);
  const auto first = vec.data();
  const auto last = vec.data() + vec.size();

  // 2/ read directly through mmaped region
  // const auto first = static_cast<const Blob*>(region.get_address());
  // const auto bytes = region.get_size();
  // const auto last = first + (bytes / sizeof(Blob));

  {
    boost::timer::auto_cpu_timer _;

    access_seq(first, last);
    // access_rnd(first, last);
  }

} catch (const std::exception& e) {
  std::fprintf(stderr, "Error: %s\n", e.what());
  return EXIT_FAILURE;
}
