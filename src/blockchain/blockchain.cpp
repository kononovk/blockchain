#include "blockchain/blockchain.hpp"

#include "utils/json.hpp"

#include <iomanip>
#include <iostream>
#include <algorithm>

namespace bc {

BlockChain& BlockChain::Push(const bc::Block& new_tail) {
  _chain.push_back(new_tail);
  return *this;
}

Block bc::BlockChain::Pop() {
  const auto res = _chain.back();
  _chain.pop_back();
  return res;
}

void BlockChain::PrintLastMessages(std::size_t n, std::ostream& os) {
  n = std::min(n, Size());
  for (auto it = _chain.rbegin(); n != 0; ++it, --n) {
    os << it->GetMessage() << std::endl;
  }
}

std::size_t BlockChain::Size() const {
  return _chain.size();
}

BlockChain BlockChain::Send(std::size_t index) const {
  BlockChain res;
  if (index < Size()) {
    auto it = _chain.begin();
    std::advance(it, index);
    for (; it != _chain.cend(); ++it) {
      res.Push(*it);
    }
  }
  return res;
}

void BlockChain::Receive(std::size_t index, const BlockChain& other) {
  for (auto i = (++index), old_size = _chain.size(); i < old_size; ++i) {
    Pop();
  }
  for (auto& block : other._chain) {
    Push(block);
  }
}

bool Sync(BlockChain& lhs, BlockChain& rhs) {
  if (lhs.Size() < rhs.Size()) {
    std::swap(lhs, rhs);
  }
  auto index = static_cast<int>(lhs.Size()) - 1;
  for (auto lit = lhs._chain.rbegin(); index != -1; --index, ++lit) {
    auto rit = rhs._chain.begin();
    for (auto i = 0llu; i < rhs.Size(); ++i, ++rit) {
      if (rit->GetHash() == lit->GetHash()) {
        auto tail = lhs.Send(index + 1);
        rhs.Receive(i, tail);
        return true;
      }
    }
  }
  return false;
}

/// friend functions ///
std::ostream& operator<<(std::ostream& os, const BlockChain& bc) {
  nlohmann::json j;
  for (auto& block : bc._chain) {
    j.push_back(block.ToJson());
  }
  return os << std::setw(4) << j;
}

std::istream& operator>>(std::istream& is, BlockChain& bc) {
  nlohmann::json j;
  is >> j;
  for (const auto& current : j) {
    bc.Push(current);
  }
  return is;
}

bool operator==(const BlockChain& lhs, const BlockChain& rhs) {
  std::stringstream ss1, ss2;
  ss1 << lhs;
  ss2 << rhs;
  return ss1.str() == ss2.str();
}

bool operator!=(const BlockChain& lhs, const BlockChain& rhs) {
  return !(lhs == rhs);
}

}  // namespace bc