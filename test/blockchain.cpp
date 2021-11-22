#include "blockchain/blockchain.hpp"

#include "block/block.hpp"

#include <cstdint>
#include <fstream>
#include <random>

#include <gtest/gtest.h>

namespace {

using namespace bc;

std::uint64_t random() {
  static std::random_device rd;
  static std::mt19937 mt(rd());
  static std::uniform_int_distribution<std::uint64_t> dist(0, std::numeric_limits<std::uint64_t>::max());
  return dist(mt);
}

Block Make(std::string message, const Block& prev) {
  return Block{std::move(message), random(), prev};
}

Block Make(std::string message) {
  return Block{std::move(message), random()};
}

TEST(BlockChain, ReadWrite) {
  Block b1 = Make("m1");
  Block b2 = Make("m2", b1);
  Block b3 = Make("m3", b2);

  BlockChain bc1{}, bc2{};
  bc1.Push(b1).Push(b2).Push(b3);

  std::stringstream ss1;
  ss1 << bc1;
  ss1 >> bc2;
  EXPECT_EQ(bc1, bc2);
}

TEST(BlockChain, Sync) {
  Block b1 = Make("b1");
  Block b2 = Make("b2", b1);
  Block b3 = Make("b3", b2);
  Block b4 = Make("b4", b3);
  Block b5 = Make("b5", b4);
  Block b6 = Make("b6", b5);

  Block b4_5("b4_5", random(), b3);

  BlockChain bc1;
  bc1.Push(b1).Push(b2).Push(b3).Push(b4).Push(b5).Push(b6);

  BlockChain bc2;
  bc2.Push(b1).Push(b2).Push(b3).Push(b4_5);

  BlockChain copy = bc1;
  EXPECT_TRUE(Sync(bc1, bc2));

  EXPECT_EQ(bc1, bc2);
  EXPECT_EQ(bc1, copy);
}

TEST(BlockChain, Sync2) {
  Block b1 = Make("m1");
  Block b2 = Make("m2", b1);
  Block b3 = Make("m3", b2);
  Block b4 = Make("m4", b3);
  Block b5 = Make("m5", b4);
  Block b6 = Make("m6", b5);

  BlockChain bc1;
  bc1.Push(b1).Push(b2).Push(b3);

  BlockChain bc2;
  bc2.Push(b1).Push(b2).Push(b3).Push(b4).Push(b5).Push(b6);

  EXPECT_TRUE(Sync(bc1, bc2));
  std::cout << bc1 << std::endl;
}

}  // namespace