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

#include "uscp/rwls.hpp"

#include <vector>

template<typename block_type_t>
void sul_dynamic_bitset_uscp_rwls(benchmark::State& state)
{
    const size_t steps = static_cast<size_t>(state.range(0));
    uscp::random_engine random_engine(SEED);

    uscp::rwls::rwls<sul::dynamic_bitset<block_type_t>> rwls(
      global::benchmark_instance<sul::dynamic_bitset<block_type_t>>);
    rwls.initialize();

    uscp::solution<sul::dynamic_bitset<block_type_t>> initial_solution =
      uscp::greedy::solve(global::benchmark_instance<sul::dynamic_bitset<block_type_t>>);

    for(auto _: state)
    {
        uscp::solution<sul::dynamic_bitset<block_type_t>> solution =
          rwls.improve(initial_solution, random_engine, steps);
        benchmark::DoNotOptimize(solution);
    }

    state.counters["1_step_time"] =
      benchmark::Counter(steps,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["steps_per_second"] =
      benchmark::Counter(steps, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

#ifdef HAS_BOOST
template<typename block_type_t>
void boost_dynamic_bitset_uscp_rwls(benchmark::State& state)
{
    const size_t steps = static_cast<size_t>(state.range(0));
    uscp::random_engine random_engine(SEED);

    uscp::rwls::rwls<boost::dynamic_bitset<block_type_t>> rwls(
      global::benchmark_instance<boost::dynamic_bitset<block_type_t>>);
    rwls.initialize();

    uscp::solution<boost::dynamic_bitset<block_type_t>> initial_solution =
      uscp::greedy::solve(global::benchmark_instance<boost::dynamic_bitset<block_type_t>>);

    for(auto _: state)
    {
        uscp::solution<boost::dynamic_bitset<block_type_t>> solution =
          rwls.improve(initial_solution, random_engine, steps);
        benchmark::DoNotOptimize(solution);
    }

    state.counters["1_step_time"] =
      benchmark::Counter(steps,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["steps_per_second"] =
      benchmark::Counter(steps, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}
#endif

#ifdef HAS_STD_TR2_DYNAMIC_BITSET
template<typename block_type_t>
void std_tr2_dynamic_bitset_uscp_rwls(benchmark::State& state)
{
    const size_t steps = static_cast<size_t>(state.range(0));
    uscp::random_engine random_engine(SEED);

    uscp::rwls::rwls<std::tr2::dynamic_bitset<block_type_t>> rwls(
      global::benchmark_instance<std::tr2::dynamic_bitset<block_type_t>>);
    rwls.initialize();

    uscp::solution<std::tr2::dynamic_bitset<block_type_t>> initial_solution =
      uscp::greedy::solve(global::benchmark_instance<std::tr2::dynamic_bitset<block_type_t>>);

    for(auto _: state)
    {
        uscp::solution<std::tr2::dynamic_bitset<block_type_t>> solution =
          rwls.improve(initial_solution, random_engine, steps);
        benchmark::DoNotOptimize(solution);
    }

    state.counters["1_step_time"] =
      benchmark::Counter(steps,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["steps_per_second"] =
      benchmark::Counter(steps, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}
#endif

inline void std_vector_bool_uscp_rwls(benchmark::State& state)
{
    const size_t steps = static_cast<size_t>(state.range(0));
    uscp::random_engine random_engine(SEED);

    uscp::rwls::rwls<std::vector<bool>> rwls(global::benchmark_instance<std::vector<bool>>);
    rwls.initialize();

    uscp::solution<std::vector<bool>> initial_solution =
      uscp::greedy::solve(global::benchmark_instance<std::vector<bool>>);

    for(auto _: state)
    {
        uscp::solution<std::vector<bool>> solution = rwls.improve(initial_solution, random_engine, steps);
        benchmark::DoNotOptimize(solution);
    }

    state.counters["1_step_time"] =
      benchmark::Counter(steps,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["steps_per_second"] =
      benchmark::Counter(steps, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}
