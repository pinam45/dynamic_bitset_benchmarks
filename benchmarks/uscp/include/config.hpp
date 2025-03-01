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

// sul::dynamic_bitset benchmark
#define SUL_DYNAMIC_BITSET_REGISTER_BENCHMARK_TEMPLATE(func, block_type, name) \
    benchmark::RegisterBenchmark("sul::dynamic_bitset<" #block_type "> " name, func<block_type>);

#define SUL_DYNAMIC_BITSET_REGISTER_BENCHMARK(func, name) \
    SUL_DYNAMIC_BITSET_REGISTER_BENCHMARK_TEMPLATE(func, uint16_t, name); \
    SUL_DYNAMIC_BITSET_REGISTER_BENCHMARK_TEMPLATE(func, uint32_t, name); \
    SUL_DYNAMIC_BITSET_REGISTER_BENCHMARK_TEMPLATE(func, uint64_t, name)

// boost::dynamic_bitset benchmark
#define BOOST_DYNAMIC_BITSET_REGISTER_BENCHMARK_TEMPLATE(func, block_type, name) \
    benchmark::RegisterBenchmark("boost::dynamic_bitset<" #block_type "> " name, func<block_type>);

#define BOOST_DYNAMIC_BITSET_REGISTER_BENCHMARK(func, name) \
    BOOST_DYNAMIC_BITSET_REGISTER_BENCHMARK_TEMPLATE(func, uint16_t, name); \
    BOOST_DYNAMIC_BITSET_REGISTER_BENCHMARK_TEMPLATE(func, uint32_t, name); \
    BOOST_DYNAMIC_BITSET_REGISTER_BENCHMARK_TEMPLATE(func, uint64_t, name)

// std::tr2::dynamic_bitset benchmark
#define STD_TR2_DYNAMIC_BITSET_REGISTER_BENCHMARK_TEMPLATE(func, block_type, name) \
    benchmark::RegisterBenchmark("std::tr2::dynamic_bitset<" #block_type "> " name, func<block_type>);

#define STD_TR2_DYNAMIC_BITSET_REGISTER_BENCHMARK(func, name) \
    STD_TR2_DYNAMIC_BITSET_REGISTER_BENCHMARK_TEMPLATE(func, uint16_t, name); \
    STD_TR2_DYNAMIC_BITSET_REGISTER_BENCHMARK_TEMPLATE(func, uint32_t, name); \
    STD_TR2_DYNAMIC_BITSET_REGISTER_BENCHMARK_TEMPLATE(func, uint64_t, name)

// std::vector<bool> benchmark
#define STD_VECTOR_BOOL_REGISTER_BENCHMARK(func, name) benchmark::RegisterBenchmark("std::vector<bool> " name, func);
