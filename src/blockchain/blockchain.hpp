#pragma once

#include "block/block.hpp"

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>

namespace bc {

class BlockChain {
 public:
  BlockChain() = default;

  BlockChain& Push(const Block&);
  Block Pop();
  std::size_t Size() const;

  void PrintLastMessages(std::size_t n, std::ostream& os = std::cout);

  BlockChain Send(std::size_t index) const;
  void Receive(std::size_t, const BlockChain&);

  friend bool Sync(BlockChain& lhs, BlockChain& rhs);
  friend std::ostream& operator<<(std::ostream&, const BlockChain&);
  friend std::istream& operator>>(std::istream&, BlockChain&);
  friend bool operator==(const BlockChain&, const BlockChain&);
  friend bool operator!=(const BlockChain&, const BlockChain&);

 private:
  std::list<Block> _chain;
};

}  // namespace bc