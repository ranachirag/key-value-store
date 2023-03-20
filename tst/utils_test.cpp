#include <gtest/gtest.h>

#include "utils.h"

TEST(MathUtils_GetPrefixBits, HandlesEqualsBitSize) {
  EXPECT_EQ(math_utils::get_prefix_bits(0, 1), 0);
  EXPECT_EQ(math_utils::get_prefix_bits(0, 2), 0);
  EXPECT_EQ(math_utils::get_prefix_bits(3, 2), 3);
  EXPECT_EQ(math_utils::get_prefix_bits(15, 4), 15);
  EXPECT_NE(math_utils::get_prefix_bits(15, 3), 15);
}

TEST(MathUtils_GetPrefixBits, HandlesSmallerBitSize) {
  // 63 in bits is 11111 (5 bits), we want first 3 bits (111), which as integer is 7
  EXPECT_EQ(math_utils::get_prefix_bits(63, 3), 7);

  // 64 in bits is 100000 (6 bits), we want first 4 bits (1000), which as integer is 8
  EXPECT_EQ(math_utils::get_prefix_bits(64, 4), 8);

  // 7 in bits is 111 (3 bits), we want first bit (1), which as integer is 1
  EXPECT_EQ(math_utils::get_prefix_bits(7, 1), 1);
}

TEST(MathUtils_GetPrefixBits, HandlesLargerBitSize) {

  // 63 in bits is 11111 (5 bits), we want first 10 bits (00000011111), not (1111100000) which is 992
  EXPECT_NE(math_utils::get_prefix_bits(63, 10), 992);

  // 63 in bits is 11111 (5 bits), we want first 10 bits (00000011111), which is same as input, 63
  EXPECT_EQ(math_utils::get_prefix_bits(63, 10), 63);

}