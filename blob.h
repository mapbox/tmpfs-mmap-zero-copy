#pragma once

#include <cstdio>
#include <cerrno>

#include <memory>
#include <vector>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <system_error>
#include <type_traits>


struct Blob {
  std::uint32_t a;
  std::uint32_t b;
};

#if not defined __GNUC__ or __GNUC__ > 4
static_assert(std::is_trivially_copyable<Blob>::value, "not safely memcpyable");
#endif

const constexpr auto kNumBlobs = 1u << 27u;


namespace std {
template <>
struct default_delete<std::FILE> {
  void operator()(std::FILE* fp) const {
    if (fp)
      (void)std::fclose(fp);
  }
};
}

using File = std::unique_ptr<std::FILE>;


inline void dump_to_binary(const char* path) {
  File out{std::fopen(path, "wb")};

  if (not out.get())
    throw std::system_error{errno, std::system_category()};

  static std::mt19937 mt{0xCAFE};
  std::uniform_int_distribution<std::uint32_t> dist;

  std::vector<Blob> buffer(kNumBlobs);
  std::generate(begin(buffer), end(buffer), [&] { return Blob{dist(mt), dist(mt)}; });

  const auto _ = std::fwrite(buffer.data(), sizeof(Blob), buffer.size(), out.get());
  (void)_;

  if (std::ferror(out.get()) != 0)
    throw std::system_error{errno, std::system_category()};

  if (std::fflush(out.get()) != 0)
    throw std::system_error{errno, std::system_category()};
}


inline auto read_from_binary(const char* path) {
  File in{std::fopen(path, "rb")};

  if (not in.get())
    throw std::system_error{errno, std::system_category()};

  std::vector<Blob> buffer(kNumBlobs);

  const auto _ = std::fread(buffer.data(), sizeof(Blob), kNumBlobs, in.get());
  (void)_;

  if (std::ferror(in.get()) != 0)
    throw std::system_error{errno, std::system_category()};

  return buffer;
}
