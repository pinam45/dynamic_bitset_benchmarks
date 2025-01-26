#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.25)

include_guard()

# Start message
message(CHECK_START "external: configuring Boost")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Check simple program compiles
set(STD_TR2_DYNAMIC_BITSET_TEST_SOURCE [=[
    #include <random>
    #include <tr2/dynamic_bitset>

    int main()
    {
        std::tr2::dynamic_bitset<uint16_t> bitset1{128, 314};
        bitset1.resize(256);

        std::minstd_rand rand(std::random_device{}());
        std::bernoulli_distribution dist;
        std::tr2::dynamic_bitset<uint16_t> bitset2{{3, 14, 159, 265, 358}};
        for(size_t i = 0; i < 12; ++i)
        {
            bitset2.push_back(dist(rand));
        }
        bitset2.append(static_cast<uint16_t>(314159));

        bitset1 &= bitset2;
        bitset1 |= bitset2;
        bitset1 ^= bitset2;
        bitset1 -= bitset2;
        bitset1 >>= 1;
        bitset1 <<= 2;

        bitset2.set(1, false);
        bitset2.reset();
        bitset1.flip();
        bitset2 = ~bitset1;

        bitset1.clear();
        bitset2.clear();

        return 0;
    }
]=])
try_compile(
  STD_TR2_DYNAMIC_BITSET_TEST_COMPILE
  SOURCE_FROM_VAR std_tr2_dynamic_bitset_test.cpp STD_TR2_DYNAMIC_BITSET_TEST_SOURCE
  CXX_STANDARD 20
)
if(STD_TR2_DYNAMIC_BITSET_TEST_COMPILE)
    set(HAS_STD_TR2_DYNAMIC_BITSET ON)
else()
    message(WARNING "std::tr2::dynamic_bitset not present or incompatible with compiler, disabled from benchmarks")
    set(HAS_STD_TR2_DYNAMIC_BITSET OFF)
endif()

# End message
list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
