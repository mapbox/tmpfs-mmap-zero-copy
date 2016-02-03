#ifndef IOSTREAMS_H_
#define IOSTREAMS_H_

#include <boost/iostreams/device/mapped_file.hpp>

#include <vector>
#include <iterator>

#include <cstdio>

#include "jujd.h"


const constexpr char* kFilePathReadable = "test-r.bin";
const constexpr char* kFilePathWriteable = "test-w.bin";


inline void mapped_source() {
  using namespace boost::iostreams;

  mapped_file_source source{kFilePathReadable};

  std::printf("open: %ju\n", ju(source.is_open()));
  std::printf("size: %ju\n", ju(source.size()));
  std::printf("alignment: %jd\n", jd(source.alignment()));

  std::vector<std::uint8_t> bytes(std::begin(source), std::end(source));
  std::printf("bytes: %ju\n", ju(bytes.size()));
}

#endif
