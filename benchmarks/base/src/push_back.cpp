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
void sul_dynamic_bitset_push_back(benchmark::State& state)
{
    const size_t bits_to_push_back = static_cast<size_t>(state.range(0));
    std::minstd_rand gen(SEED);
    std::bernoulli_distribution d;
    std::vector<bool> values;
    values.reserve(bits_to_push_back);
    for(size_t i = 0; i < bits_to_push_back; ++i)
    {
        values.push_back(d(gen));
    }

    for(auto _: state)
    {
        // setup
        sul::dynamic_bitset<block_type_t> bitset;

        // run
        for(const bool value: values)
        {
            bitset.push_back(value);
        }
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(bits_to_push_back,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] = benchmark::Counter(
      bits_to_push_back, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

SUL_DYNAMIC_BITSET_BENCHMARK_RANGE(sul_dynamic_bitset_push_back, "push_back");

#ifdef HAS_BOOST
template<typename block_type_t>
void boost_dynamic_bitset_push_back(benchmark::State& state)
{
    const size_t bits_to_push_back = static_cast<size_t>(state.range(0));
    std::minstd_rand gen(SEED);
    std::bernoulli_distribution d;
    std::vector<bool> values;
    values.reserve(bits_to_push_back);
    for(size_t i = 0; i < bits_to_push_back; ++i)
    {
        values.push_back(d(gen));
    }

    for(auto _: state)
    {
        // setup
        boost::dynamic_bitset<block_type_t> bitset;

        // run
        for(const bool value: values)
        {
            bitset.push_back(value);
        }
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(bits_to_push_back,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] = benchmark::Counter(
      bits_to_push_back, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

BOOST_DYNAMIC_BITSET_BENCHMARK_RANGE(boost_dynamic_bitset_push_back, "push_back");
#endif

#ifdef HAS_STD_TR2_DYNAMIC_BITSET
template<typename block_type_t>
void std_tr2_dynamic_bitset_push_back(benchmark::State& state)
{
    const size_t bits_to_push_back = static_cast<size_t>(state.range(0));
    std::minstd_rand gen(SEED);
    std::bernoulli_distribution d;
    std::vector<bool> values;
    values.reserve(bits_to_push_back);
    for(size_t i = 0; i < bits_to_push_back; ++i)
    {
        values.push_back(d(gen));
    }

    for(auto _: state)
    {
        // setup
        std::tr2::dynamic_bitset<block_type_t> bitset;

        // run
        for(const bool value: values)
        {
            bitset.push_back(value);
        }
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(bits_to_push_back,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] = benchmark::Counter(
      bits_to_push_back, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

STD_TR2_DYNAMIC_BITSET_BENCHMARK_RANGE(std_tr2_dynamic_bitset_push_back, "push_back");
#endif

void std_vector_bool_push_back(benchmark::State& state)
{
    const size_t bits_to_push_back = static_cast<size_t>(state.range(0));
    std::minstd_rand gen(SEED);
    std::bernoulli_distribution d;
    std::vector<bool> values;
    values.reserve(bits_to_push_back);
    for(size_t i = 0; i < bits_to_push_back; ++i)
    {
        values.push_back(d(gen));
    }

    for(auto _: state)
    {
        // setup
        std::vector<bool> bitset;

        // run
        for(const bool value: values)
        {
            bitset.push_back(value);
        }
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(bits_to_push_back,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] = benchmark::Counter(
      bits_to_push_back, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

STD_VECTOR_BOOL_BENCHMARK_RANGE(std_vector_bool_push_back, "push_back");
