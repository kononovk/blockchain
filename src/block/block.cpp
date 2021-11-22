#include "block/block.hpp"

#include "utils/hash_combine.hpp"
#include "utils/json.hpp"

#include <functional>
#include <utility>

namespace bc {
namespace {

auto TimeNow() {
  return std::time(nullptr);
}

}

Block::Block(std::string message, const std::uint64_t nonce)
    : message_{std::move(message)}, nonce_{nonce}, hash_{0}, timestamp_{TimeNow()} {
}

Block::Block(std::string message, const std::uint64_t nonce, const bc::Block& block)
    : message_{std::move(message)}, nonce_{nonce}, hash_{block.GetHash()}, timestamp_{TimeNow()} {
  utils::hash_combine(hash_, std::hash<std::string>{}(message_), std::hash<std::time_t>{}(timestamp_),
                      std::hash<std::uint64_t>{}(nonce_));
}

Block::Block(std::string message, std::uint64_t nonce, std::uint64_t hash, std::time_t timestamp)
    : message_{std::move(message)}, nonce_{nonce}, hash_{hash}, timestamp_{timestamp} {
}

Block::Block(const nlohmann::json& data) {
  data.at("message").get_to(message_);
  data.at("nonce").get_to(nonce_);
  data.at("hash").get_to(hash_);
  data.at("timestamp").get_to(timestamp_);
}

Block::Block(nlohmann::json&& data)
    : message_{std::move(data["message"])}, nonce_{data["nonce"]}, hash_{data["hash"]}, timestamp_{data["timestamp"]} {
  data.clear();
}

nlohmann::json Block::ToJson() const {
  return nlohmann::json{{"message", message_}, {"nonce", nonce_}, {"hash", hash_}, {"timestamp", timestamp_}};
}

std::string Block::GetMessage() const {
  return message_;
}

std::uint64_t Block::GetHash() const {
  return hash_;
}

std::time_t Block::GetTimestamp() const {
  return timestamp_;
}

std::uint64_t Block::GetNonce() const {
  return nonce_;
}

}  // namespace bc