#include <exception>

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <boost/timer/timer.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include "file_mapping.h"
#include "mincore.h"
#include "blob.h"
#include "access.h"

static const constexpr std::size_t MAX_RUNS = 10;

template <typename Iter>
void
run_access(const char* pattern, Iter first, Iter last) {
  std::string result;

  if (pattern == std::string("seq")) {
    boost::timer::cpu_timer accessing;
    for (auto idx = MAX_RUNS; idx > 0; --idx)
      access_seq(first, last);
    accessing.stop();
    result = accessing.format();
  } else if (pattern == std::string("rnd")) {
    boost::timer::cpu_timer accessing;
    for (auto idx = MAX_RUNS; idx > 0; --idx)
      access_rnd(first, last);
    accessing.stop();
    result = accessing.format();
  } else {
    std::printf("Error: invalid access pattern: %s\n", pattern);
    return;
  }

  std::printf("Accessing: %s", result.c_str());
}

int
main(int argc, char** argv) try {
  if (argc != 4)
    return EXIT_FAILURE;

  auto memory_type = argv[1];
  auto access_pattern = argv[2];
  boost::filesystem::path path(argv[3]);

  if (!boost::filesystem::exists(path)) {
    std::printf("Dumping to %s...", path.c_str());
    dump_to_binary(path.string().c_str());
    std::printf(" ok.\n");
  }


  if (memory_type == std::string("heap")) {
    // 1/ read from file into heap
    boost::timer::cpu_timer loading;
    const auto vec = read_from_binary(path.c_str());
    const auto first = vec.data();
    const auto last = vec.data() + vec.size();
    loading.stop();
    std::printf("Loading: %s", loading.format().c_str());
    //std::printf("Waiting before load...");
    //std::cout << std::flush;
    //sleep(10);
    //std::printf(" ok.\n");
    run_access(access_pattern, first, last);
  } else if (memory_type == std::string("mmap")) {
    // 2/ read directly through mmaped region
    boost::timer::cpu_timer loading;
    const auto region = make_file_mapping(path.c_str());
    const auto first = static_cast<const Blob*>(region.get_address());
    const auto bytes = region.get_size();
    const auto last = first + (bytes / sizeof(Blob));
    loading.stop();
    std::printf("Loading: %s", loading.format().c_str());
    //std::printf("Waiting before load...");
    //std::cout << std::flush;
    //sleep(10);
    //std::printf(" ok.\n");
    run_access(access_pattern, first, last);
  } else {
    std::printf("Error: invalid memory type %s\n", memory_type);
    std::printf("./core [heap|mmap] [seq|rnd] PATH_TO_FILE");
    return EXIT_FAILURE;
  }

} catch (const std::exception& e) {
  std::fprintf(stderr, "Error: %s\n", e.what());
  return EXIT_FAILURE;
}
