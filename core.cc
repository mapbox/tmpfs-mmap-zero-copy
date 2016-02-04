#include <exception>

#include <cstdio>
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

  boost::timer::cpu_timer loading;

  // 1/ read from file into heap
  // const auto vec = read_from_binary(argv[1]);
  // const auto first = vec.data();
  // const auto last = vec.data() + vec.size();

  // 2/ read directly through mmaped region
  const auto region = make_file_mapping(argv[1]);
  const auto first = static_cast<const Blob*>(region.get_address());
  const auto bytes = region.get_size();
  const auto last = first + (bytes / sizeof(Blob));

  loading.stop();
  std::printf("Loading: %s", loading.format().c_str());

  boost::timer::cpu_timer accessing;

  access_seq(first, last);
  // access_rnd(first, last);

  accessing.stop();
  std::printf("Accessing: %s", accessing.format().c_str());

} catch (const std::exception& e) {
  std::fprintf(stderr, "Error: %s\n", e.what());
  return EXIT_FAILURE;
}
