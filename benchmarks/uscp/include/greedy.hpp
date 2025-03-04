//
// Copyright (c) 2025 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include "global.hpp"
#include "uscp/greedy.hpp"

#include <benchmark/benchmark.h>

#include <sul/dynamic_bitset.hpp>
#ifdef HAS_BOOST
#    include <boost/dynamic_bitset.hpp>
#endif
#ifdef HAS_STD_TR2_DYNAMIC_BITSET
#    include <tr2/dynamic_bitset>
#endif

#include <vector>

template<typename block_type_t>
void sul_dynamic_bitset_uscp_greedy(benchmark::State& state)
{
    for(auto _: state)
    {
        uscp::solution<sul::dynamic_bitset<block_type_t>> solution =
          uscp::greedy::solve(global::benchmark_instance<sul::dynamic_bitset<block_type_t>>);
        benchmark::DoNotOptimize(solution);
    }
}

#ifdef HAS_BOOST
template<typename block_type_t>
void boost_dynamic_bitset_uscp_greedy(benchmark::State& state)
{
    for(auto _: state)
    {
        uscp::solution<boost::dynamic_bitset<block_type_t>> solution =
          uscp::greedy::solve(global::benchmark_instance<boost::dynamic_bitset<block_type_t>>);
        benchmark::DoNotOptimize(solution);
    }
}
#endif

#ifdef HAS_STD_TR2_DYNAMIC_BITSET
template<typename block_type_t>
void std_tr2_dynamic_bitset_uscp_greedy(benchmark::State& state)
{
    for(auto _: state)
    {
        uscp::solution<std::tr2::dynamic_bitset<block_type_t>> solution =
          uscp::greedy::solve(global::benchmark_instance<std::tr2::dynamic_bitset<block_type_t>>);
        benchmark::DoNotOptimize(solution);
    }
}
#endif

inline void std_vector_bool_uscp_greedy(benchmark::State& state)
{
    for(auto _: state)
    {
        uscp::solution<std::vector<bool>> solution = uscp::greedy::solve(global::benchmark_instance<std::vector<bool>>);
        benchmark::DoNotOptimize(solution);
    }
}
