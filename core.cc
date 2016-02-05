#include <exception>
#include <string>

#include <cstdio>
#include <cstdlib>

#include <boost/timer/timer.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include "mapping.h"
#include "mincore.h"
#include "blob.h"
#include "access.h"


const constexpr auto kMaxRuns = 10u;

template <typename Iter>
void run_access(const char* pattern, Iter first, Iter last) {
  using namespace std::string_literals;

  std::string result;

  if (pattern == "seq"s) {
    boost::timer::cpu_timer accessing;
    for (auto idx = kMaxRuns; idx != 0; --idx)
      access_seq(first, last);
    accessing.stop();
    result = accessing.format();
  } else if (pattern == "rnd"s) {
    boost::timer::cpu_timer accessing;
    for (auto idx = kMaxRuns; idx != 0; --idx)
      access_rnd(first, last);
    accessing.stop();
    result = accessing.format();
  } else {
    std::fprintf(stderr, "Error: invalid access pattern: %s\n", pattern);
    return;
  }

  std::printf("Accessing: %s", result.c_str());
}

inline void usage(const char* path) { std::fprintf(stderr, "Usage: %s [ heap | mmap ] [ seq | rnd ] path\n", path); }


int main(int argc, char** argv) try {
  if (argc != 4) {
    usage(argv[0]);
    return EXIT_FAILURE;
  }

  const auto memory_type = argv[1];
  const auto access_pattern = argv[2];
  const boost::filesystem::path path(argv[3]);

  if (not boost::filesystem::exists(path)) {
    std::printf("Dumping to %s\n", path.c_str());
    dump_to_binary(path.string().c_str());
  }

  using namespace std::string_literals;

  if (memory_type == "heap"s) {
    // 1/ read from file into heap
    boost::timer::cpu_timer loading;
    const auto vec = read_from_binary(path.c_str());
    //const auto vec = read_from_binary_through_stream(path.c_str());
    const auto first = vec.data();
    const auto last = vec.data() + vec.size();
    loading.stop();
    std::printf("Loading: %s", loading.format().c_str());
    run_access(access_pattern, first, last);
  } else if (memory_type == "mmap"s) {
    // 2/ read directly through mmaped region
    boost::timer::cpu_timer loading;
    const auto region = make_mapping(path.c_str());
    const auto first = static_cast<const Blob*>(region.get_address());
    const auto bytes = region.get_size();
    const auto last = first + (bytes / sizeof(Blob));
    loading.stop();
    std::printf("Loading: %s", loading.format().c_str());
    run_access(access_pattern, first, last);
  } else {
    std::fprintf(stderr, "Error: invalid memory type %s\n", memory_type);
    usage(argv[0]);
    return EXIT_FAILURE;
  }

} catch (const std::exception& e) {
  std::fprintf(stderr, "Error: %s\n", e.what());
  return EXIT_FAILURE;
}
