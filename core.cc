#include <exception>

#include <cstdlib>

#include <boost/timer/timer.hpp>

#include "file_mapping.h"
#include "mincore.h"
#include "blob.h"
#include "access.h"


int main(int argc, char** argv) try {
  if (argc != 2)
    return EXIT_FAILURE;

  // dump_to_binary(argv[1]);

  // 1/ read from file into heap
  // const auto vec = read_from_binary(argv[1]);
  // const auto first = vec.data();
  // const auto last = vec.data() + vec.size();

  // 2/ read directly through mmaped region
  const auto region = make_file_mapping(argv[1]);

  // dump_mincore(region);

  const auto first = static_cast<const Blob*>(region.get_address());
  const auto bytes = region.get_size();
  const auto last = first + (bytes / sizeof(Blob));

  {
    boost::timer::auto_cpu_timer _;

    // access_seq(first, last);
    access_rnd(first, last);
  }

} catch (const std::exception& e) {
  std::fprintf(stderr, "Error: %s\n", e.what());
  return EXIT_FAILURE;
}
