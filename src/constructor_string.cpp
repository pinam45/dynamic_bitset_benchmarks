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
#include <sstream>

template<typename block_type_t>
void sul_dynamic_bitset_constructor_string(benchmark::State& state)
{
    const size_t bits = static_cast<size_t>(state.range(0));
    std::minstd_rand gen(SEED);
    std::bernoulli_distribution d;
    std::ostringstream oss;
    for(size_t i = 0; i < bits; ++i)
    {
        oss << (d(gen) ? '1' : '0');
    }
    const std::string str = oss.str();
    benchmark::ClobberMemory();

    for(auto _: state)
    {
        sul::dynamic_bitset<block_type_t> bitset(str);
        benchmark::DoNotOptimize(bitset);
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(bits,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] =
      benchmark::Counter(bits, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

SUL_DYNAMIC_BITSET_BENCHMARK_RANGE(sul_dynamic_bitset_constructor_string, "constructor_string");

#ifdef HAS_BOOST
template<typename block_type_t>
void boost_dynamic_bitset_constructor_string(benchmark::State& state)
{
    const size_t bits = static_cast<size_t>(state.range(0));
    std::minstd_rand gen(SEED);
    std::bernoulli_distribution d;
    std::ostringstream oss;
    for(size_t i = 0; i < bits; ++i)
    {
        oss << (d(gen) ? '1' : '0');
    }
    const std::string str = oss.str();
    benchmark::ClobberMemory();

    for(auto _: state)
    {
        boost::dynamic_bitset<block_type_t> bitset(str);
        benchmark::DoNotOptimize(bitset);
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(bits,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] =
      benchmark::Counter(bits, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

BOOST_DYNAMIC_BITSET_BENCHMARK_RANGE(boost_dynamic_bitset_constructor_string, "constructor_string");
#endif

#ifdef HAS_STD_TR2_DYNAMIC_BITSET
template<typename block_type_t>
void std_tr2_dynamic_bitset_constructor_string(benchmark::State& state)
{
    const size_t bits = static_cast<size_t>(state.range(0));
    std::minstd_rand gen(SEED);
    std::bernoulli_distribution d;
    std::ostringstream oss;
    for(size_t i = 0; i < bits; ++i)
    {
        oss << (d(gen) ? '1' : '0');
    }
    const std::string str = oss.str();
    benchmark::ClobberMemory();

    for(auto _: state)
    {
        std::tr2::dynamic_bitset<block_type_t> bitset(str);
        benchmark::DoNotOptimize(bitset);
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(bits,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] =
      benchmark::Counter(bits, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

STD_TR2_DYNAMIC_BITSET_BENCHMARK_RANGE(std_tr2_dynamic_bitset_constructor_string, "constructor_string");
#endif

template<size_t size>
void std_bitset_constructor_string(benchmark::State& state)
{
    constexpr size_t bits = size;
    std::minstd_rand gen(SEED);
    std::bernoulli_distribution d;
    std::ostringstream oss;
    for(size_t i = 0; i < bits; ++i)
    {
        oss << (d(gen) ? '1' : '0');
    }
    const std::string str = oss.str();
    benchmark::ClobberMemory();

    for(auto _: state)
    {
        std::bitset<size> bitset(str);
        benchmark::DoNotOptimize(bitset);
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(bits,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] =
      benchmark::Counter(bits, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

STD_BITSET_BENCHMARK_RANGE(std_bitset_constructor_string, "constructor_string");
