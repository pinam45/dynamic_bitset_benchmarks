//
// Copyright (c) 2025 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include <random>

const std::minstd_rand::result_type SEED = std::random_device{}();
static constexpr size_t RANGE_START = 64ull;
static constexpr size_t RANGE_END = 1ull << 20u;
static constexpr size_t RANGE_MULTIPLIER = 1ull << 2u;

// sul::dynamic_bitset benchmark
#define SUL_DYNAMIC_BITSET_BENCHMARK_TEMPLATE(func, block_type, name) \
    BENCHMARK_TEMPLATE(sul_dynamic_bitset_and_equal, block_type) \
      ->Name("sul::dynamic_bitset<" #block_type "> " name) \
      ->RangeMultiplier(RANGE_MULTIPLIER) \
      ->Range(RANGE_START, RANGE_END);

#define SUL_DYNAMIC_BITSET_BENCHMARK(func, name) \
    SUL_DYNAMIC_BITSET_BENCHMARK_TEMPLATE(func, uint16_t, name); \
    SUL_DYNAMIC_BITSET_BENCHMARK_TEMPLATE(func, uint32_t, name); \
    SUL_DYNAMIC_BITSET_BENCHMARK_TEMPLATE(func, uint64_t, name)

// boost::dynamic_bitset benchmark
#define BOOST_DYNAMIC_BITSET_BENCHMARK_TEMPLATE(func, block_type, name) \
    BENCHMARK_TEMPLATE(sul_dynamic_bitset_and_equal, block_type) \
      ->Name("boost::dynamic_bitset<" #block_type "> " name) \
      ->RangeMultiplier(RANGE_MULTIPLIER) \
      ->Range(RANGE_START, RANGE_END);

#define BOOST_DYNAMIC_BITSET_BENCHMARK(func, name) \
    BOOST_DYNAMIC_BITSET_BENCHMARK_TEMPLATE(func, uint16_t, name); \
    BOOST_DYNAMIC_BITSET_BENCHMARK_TEMPLATE(func, uint32_t, name); \
    BOOST_DYNAMIC_BITSET_BENCHMARK_TEMPLATE(func, uint64_t, name)

// std::tr2::dynamic_bitset benchmark
#define STD_TR2_DYNAMIC_BITSET_BENCHMARK_TEMPLATE(func, block_type, name) \
    BENCHMARK_TEMPLATE(sul_dynamic_bitset_and_equal, block_type) \
      ->Name("std::tr2::dynamic_bitset<" #block_type "> " name) \
      ->RangeMultiplier(RANGE_MULTIPLIER) \
      ->Range(RANGE_START, RANGE_END);

#define STD_TR2_DYNAMIC_BITSET_BENCHMARK(func, name) \
    STD_TR2_DYNAMIC_BITSET_BENCHMARK_TEMPLATE(func, uint16_t, name); \
    STD_TR2_DYNAMIC_BITSET_BENCHMARK_TEMPLATE(func, uint32_t, name); \
    STD_TR2_DYNAMIC_BITSET_BENCHMARK_TEMPLATE(func, uint64_t, name)

// std::bitset benchmark
#define STD_BITSET_BENCHMARK_TEMPLATE(func, bits, name) \
    BENCHMARK_TEMPLATE(func, bits)->Name("std::bitset<" #bits "> " name)

#define STD_BITSET_BENCHMARK(func, name) \
    STD_BITSET_BENCHMARK_TEMPLATE(func, 64, name); \
    STD_BITSET_BENCHMARK_TEMPLATE(func, 256, name); \
    STD_BITSET_BENCHMARK_TEMPLATE(func, 1024, name); \
    STD_BITSET_BENCHMARK_TEMPLATE(func, 4096, name); \
    STD_BITSET_BENCHMARK_TEMPLATE(func, 16384, name); \
    STD_BITSET_BENCHMARK_TEMPLATE(func, 65536, name); \
    STD_BITSET_BENCHMARK_TEMPLATE(func, 262144, name); \
    STD_BITSET_BENCHMARK_TEMPLATE(func, 1048576, name)
