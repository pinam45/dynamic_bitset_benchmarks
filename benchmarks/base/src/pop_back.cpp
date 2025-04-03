//
// Copyright (c) 2025 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include <config.hpp>
#include <utils.hpp>

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
void sul_dynamic_bitset_pop_back(benchmark::State& state)
{
    const size_t bits_to_pop_back = static_cast<size_t>(state.range(0));
    std::minstd_rand gen(SEED);

    for(auto _: state)
    {
        // setup
        state.PauseTiming();
        sul::dynamic_bitset<block_type_t> bitset =
          random_bitset<sul::dynamic_bitset<block_type_t>>(gen, bits_to_pop_back);
        benchmark::ClobberMemory();
        state.ResumeTiming();

        // run
        for(size_t i = 0; i < bits_to_pop_back; ++i)
        {
            bitset.pop_back();
        }
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(bits_to_pop_back,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] = benchmark::Counter(
      bits_to_pop_back, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

SUL_DYNAMIC_BITSET_BENCHMARK_RANGE(sul_dynamic_bitset_pop_back, "pop_back");

#ifdef HAS_BOOST
template<typename block_type_t>
void boost_dynamic_bitset_pop_back(benchmark::State& state)
{
    const size_t bits_to_pop_back = static_cast<size_t>(state.range(0));
    std::minstd_rand gen(SEED);

    for(auto _: state)
    {
        // setup
        state.PauseTiming();
        boost::dynamic_bitset<block_type_t> bitset =
          random_bitset<boost::dynamic_bitset<block_type_t>>(gen, bits_to_pop_back);
        benchmark::ClobberMemory();
        state.ResumeTiming();

        // run
        for(size_t i = 0; i < bits_to_pop_back; ++i)
        {
            bitset.pop_back();
        }
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(bits_to_pop_back,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] = benchmark::Counter(
      bits_to_pop_back, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

BOOST_DYNAMIC_BITSET_BENCHMARK_RANGE(boost_dynamic_bitset_pop_back, "pop_back");
#endif

void std_vector_bool_pop_back(benchmark::State& state)
{
    const size_t bits_to_pop_back = static_cast<size_t>(state.range(0));
    std::minstd_rand gen(SEED);

    for(auto _: state)
    {
        // setup
        state.PauseTiming();
        std::vector<bool> bitset = random_bitset<std::vector<bool>>(gen, bits_to_pop_back);
        benchmark::ClobberMemory();
        state.ResumeTiming();

        // run
        for(size_t i = 0; i < bits_to_pop_back; ++i)
        {
            bitset.pop_back();
        }
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(bits_to_pop_back,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] = benchmark::Counter(
      bits_to_pop_back, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

STD_VECTOR_BOOL_BENCHMARK_RANGE(std_vector_bool_pop_back, "pop_back");
