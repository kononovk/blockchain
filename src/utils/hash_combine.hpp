#pragma once

#include <type_traits>
#include <concepts>

namespace bc::utils {

static_assert(sizeof(std::size_t) == sizeof(std::uint64_t));

// hash combine from boost hash
inline void hash_combine(std::size_t& seed, std::size_t value) {
  static constexpr std::uint64_t m = 0xc6a4a7935bd1e995UL;
  static constexpr int r = 47;

  value *= m;
  value ^= value >> r;
  value *= m;

  seed ^= value;
  seed *= m;

  seed += 0xe6546b64;
}

template <std::same_as<std::size_t> ... Args>
inline void hash_combine(std::size_t& hash_first, Args... hashes) {
  (hash_combine(hash_first, hashes), ...);
}

}  // namespace bc::utils

