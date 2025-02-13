//
// Copyright (c) 2025 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include <config.hpp>

#include <benchmark/benchmark.h>
#include <sul/dynamic_bitset.hpp>
#ifdef HAS_BOOST
#    include <boost/dynamic_bitset.hpp>
#endif
#ifdef HAS_STD_TR2_DYNAMIC_BITSET
#    include <tr2/dynamic_bitset>
#endif

#include <bitset>
#include <chrono>
#include <random>
#include <vector>

template<typename block_type_t>
void sul_dynamic_bitset_constructor_default(benchmark::State& state)
{
    for(auto _: state)
    {
        sul::dynamic_bitset<block_type_t> bitset;
        benchmark::DoNotOptimize(bitset);
        benchmark::ClobberMemory();
    }
}

SUL_DYNAMIC_BITSET_BENCHMARK(sul_dynamic_bitset_constructor_default, "constructor_default");

#ifdef HAS_BOOST
template<typename block_type_t>
void boost_dynamic_bitset_constructor_default(benchmark::State& state)
{
    for(auto _: state)
    {
        boost::dynamic_bitset<block_type_t> bitset;
        benchmark::DoNotOptimize(bitset);
        benchmark::ClobberMemory();
    }
}

BOOST_DYNAMIC_BITSET_BENCHMARK(boost_dynamic_bitset_constructor_default, "constructor_default");
#endif

#ifdef HAS_STD_TR2_DYNAMIC_BITSET
template<typename block_type_t>
void std_tr2_dynamic_bitset_constructor_default(benchmark::State& state)
{
    for(auto _: state)
    {
        std::tr2::dynamic_bitset<block_type_t> bitset;
        benchmark::DoNotOptimize(bitset);
        benchmark::ClobberMemory();
    }
}

STD_TR2_DYNAMIC_BITSET_BENCHMARK(std_tr2_dynamic_bitset_constructor_default, "constructor_default");
#endif

void std_vector_bool_constructor_default(benchmark::State& state)
{
    for(auto _: state)
    {
        std::vector<bool> bitset;
        benchmark::DoNotOptimize(bitset);
        benchmark::ClobberMemory();
    }
}

STD_VECTOR_BOOL_BENCHMARK(std_vector_bool_constructor_default, "constructor_default");
