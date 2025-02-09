//
// Copyright (c) 2025 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include "config.hpp"
#include "utils.hpp"

#include <benchmark/benchmark.h>
#include <sul/dynamic_bitset.hpp>
#ifdef HAS_STD_TR2_DYNAMIC_BITSET
#    include <tr2/dynamic_bitset>
#endif

#include <bitset>
#include <chrono>
#include <random>

template<typename block_type_t>
void sul_dynamic_bitset_constructor_initializer_list(benchmark::State& state)
{
    std::minstd_rand gen(SEED);
    std::uniform_int_distribution<block_type_t> dis;
    const block_type_t init_val_1 = dis(gen);
    const block_type_t init_val_2 = dis(gen);
    const block_type_t init_val_3 = dis(gen);
    const block_type_t init_val_4 = dis(gen);

    for(auto _: state)
    {
        sul::dynamic_bitset<block_type_t> bitset({init_val_1, init_val_2, init_val_3, init_val_4});
        benchmark::DoNotOptimize(bitset);
        benchmark::ClobberMemory();
    }
}

SUL_DYNAMIC_BITSET_BENCHMARK(sul_dynamic_bitset_constructor_initializer_list, "constructor_initializer_list");

#ifdef HAS_STD_TR2_DYNAMIC_BITSET
template<typename block_type_t>
void std_tr2_dynamic_bitset_constructor_initializer_list(benchmark::State& state)
{
    std::minstd_rand gen(SEED);
    std::uniform_int_distribution<block_type_t> dis;
    const block_type_t init_val_1 = dis(gen);
    const block_type_t init_val_2 = dis(gen);
    const block_type_t init_val_3 = dis(gen);
    const block_type_t init_val_4 = dis(gen);

    for(auto _: state)
    {
        std::tr2::dynamic_bitset<block_type_t> bitset({init_val_1, init_val_2, init_val_3, init_val_4});
        benchmark::DoNotOptimize(bitset);
        benchmark::ClobberMemory();
    }
}

STD_TR2_DYNAMIC_BITSET_BENCHMARK(std_tr2_dynamic_bitset_constructor_initializer_list, "constructor_initializer_list");
#endif
