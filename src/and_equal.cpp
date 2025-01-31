//
// Copyright (c) 2025 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include "config.hpp"

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

template<typename block_type_t>
void sul_dynamic_bitset_and_equal(benchmark::State& state)
{
    const size_t bits_to_and = static_cast<size_t>(state.range(0));
    std::minstd_rand gen(SEED);
    std::bernoulli_distribution d;
    sul::dynamic_bitset<block_type_t> bitset1;
    sul::dynamic_bitset<block_type_t> bitset2;
    bitset1.reserve(bits_to_and);
    bitset2.reserve(bits_to_and);
    for(size_t i = 0; i < bits_to_and; ++i)
    {
        bitset1.push_back(d(gen));
        bitset2.push_back(d(gen));
    }
    benchmark::ClobberMemory();

    for(auto _: state)
    {
        benchmark::DoNotOptimize(bitset1 &= bitset2);
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(bits_to_and,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] =
      benchmark::Counter(bits_to_and, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

SUL_DYNAMIC_BITSET_BENCHMARK(sul_dynamic_bitset_and_equal, "&=");

#ifdef HAS_BOOST
template<typename block_type_t>
void boost_dynamic_bitset_and_equal(benchmark::State& state)
{
    const size_t bits_to_and = static_cast<size_t>(state.range(0));
    std::minstd_rand gen(SEED);
    std::bernoulli_distribution d;
    boost::dynamic_bitset<block_type_t> bitset1;
    boost::dynamic_bitset<block_type_t> bitset2;
    bitset1.reserve(bits_to_and);
    bitset2.reserve(bits_to_and);
    for(size_t i = 0; i < bits_to_and; ++i)
    {
        bitset1.push_back(d(gen));
        bitset2.push_back(d(gen));
    }
    benchmark::ClobberMemory();

    for(auto _: state)
    {
        benchmark::DoNotOptimize(bitset1 &= bitset2);
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(bits_to_and,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] =
      benchmark::Counter(bits_to_and, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

BOOST_DYNAMIC_BITSET_BENCHMARK(boost_dynamic_bitset_and_equal, "&=");
#endif

#ifdef HAS_STD_TR2_DYNAMIC_BITSET
template<typename block_type_t>
void std_tr2_dynamic_bitset_and_equal(benchmark::State& state)
{
    const size_t bits_to_and = static_cast<size_t>(state.range(0));
    std::minstd_rand gen(SEED);
    std::bernoulli_distribution d;
    std::tr2::dynamic_bitset<block_type_t> bitset1;
    std::tr2::dynamic_bitset<block_type_t> bitset2;
    bitset1.reserve(bits_to_and);
    bitset2.reserve(bits_to_and);
    for(size_t i = 0; i < bits_to_and; ++i)
    {
        bitset1.push_back(d(gen));
        bitset2.push_back(d(gen));
    }
    benchmark::ClobberMemory();

    for(auto _: state)
    {
        benchmark::DoNotOptimize(bitset1 &= bitset2);
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(bits_to_and,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] =
      benchmark::Counter(bits_to_and, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

STD_TR2_DYNAMIC_BITSET_BENCHMARK(std_tr2_dynamic_bitset_and_equal, "&=");
#endif

template<size_t size>
void std_bitset_and_equal(benchmark::State& state)
{
    std::minstd_rand gen(SEED);
    std::bernoulli_distribution d;
    std::bitset<size> bitset1;
    std::bitset<size> bitset2;

    for(size_t i = 0; i < size; ++i)
    {
        bitset1[i] = d(gen);
        bitset2[i] = d(gen);
    }
    benchmark::ClobberMemory();

    for(auto _: state)
    {
        benchmark::DoNotOptimize(bitset1 &= bitset2);
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(size,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] =
      benchmark::Counter(size, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

STD_BITSET_BENCHMARK(std_bitset_and_equal, "&=");
