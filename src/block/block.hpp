#pragma once

#include "utils/json.hpp"

#include <cstdint>
#include <ctime>
#include <iostream>
#include <string>

namespace bc {

class Block final {
 public:
  Block() = default;
  Block(std::string message, std::uint64_t nonce);
  Block(std::string message, std::uint64_t nonce, const Block&);
  Block(const nlohmann::json&);
  Block(nlohmann::json&&);


  nlohmann::json ToJson() const;

  std::string GetMessage() const;
  std::uint64_t GetNonce() const;
  std::uint64_t GetHash() const;
  std::time_t GetTimestamp() const;

 private:
  Block(std::string message, std::uint64_t nonce, std::uint64_t hash, std::time_t timestamp);

  std::string message_{};    /// Data stored in one block of blockchain
  std::uint64_t nonce_{0};    /// Parameter for the PoW
  std::uint64_t hash_{0};  /// Hash of previous block, 0 for the first block
  std::time_t timestamp_;  /// Timestamp when block was created
};

}  // namespace bc