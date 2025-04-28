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
#include <ranges>
#include <vector>

template<typename block_type_t>
void sul_dynamic_bitset_count(benchmark::State& state)
{
    const size_t bits = static_cast<size_t>(state.range(0));
    std::minstd_rand gen(SEED);
    sul::dynamic_bitset<block_type_t> bitset = random_bitset<sul::dynamic_bitset<block_type_t>>(gen, bits);
    benchmark::ClobberMemory();

    for(auto _: state)
    {
        benchmark::DoNotOptimize(bitset.count());
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(bits,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] =
      benchmark::Counter(bits, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

SUL_DYNAMIC_BITSET_BENCHMARK_RANGE(sul_dynamic_bitset_count, "count");

#ifdef HAS_BOOST
template<typename block_type_t>
void boost_dynamic_bitset_count(benchmark::State& state)
{
    const size_t bits = static_cast<size_t>(state.range(0));
    std::minstd_rand gen(SEED);
    boost::dynamic_bitset<block_type_t> bitset = random_bitset<boost::dynamic_bitset<block_type_t>>(gen, bits);
    benchmark::ClobberMemory();

    for(auto _: state)
    {
        benchmark::DoNotOptimize(bitset.count());
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(bits,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] =
      benchmark::Counter(bits, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

BOOST_DYNAMIC_BITSET_BENCHMARK_RANGE(boost_dynamic_bitset_count, "count");
#endif

#ifdef HAS_STD_TR2_DYNAMIC_BITSET
template<typename block_type_t>
void std_tr2_dynamic_bitset_count(benchmark::State& state)
{
    const size_t bits = static_cast<size_t>(state.range(0));
    std::minstd_rand gen(SEED);
    std::tr2::dynamic_bitset<block_type_t> bitset = random_bitset<std::tr2::dynamic_bitset<block_type_t>>(gen, bits);
    benchmark::ClobberMemory();

    for(auto _: state)
    {
        benchmark::DoNotOptimize(bitset.count());
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(bits,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] =
      benchmark::Counter(bits, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

STD_TR2_DYNAMIC_BITSET_BENCHMARK_RANGE(std_tr2_dynamic_bitset_count, "count");
#endif

void std_vector_bool_count(benchmark::State& state)
{
    const size_t bits = static_cast<size_t>(state.range(0));
    std::minstd_rand gen(SEED);
    std::vector<bool> bitset = random_bitset<std::vector<bool>>(gen, bits);
    benchmark::ClobberMemory();

    for(auto _: state)
    {
        for(size_t i = 0; i < bits; ++i)
        {
            benchmark::DoNotOptimize(std::ranges::count(bitset, true));
        }
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(bits,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] =
      benchmark::Counter(bits, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

STD_VECTOR_BOOL_BENCHMARK_RANGE(std_vector_bool_count, "count");

template<size_t bits>
void std_bitset_count(benchmark::State& state)
{
    std::minstd_rand gen(SEED);
    std::bitset<bits> bitset = random_std_bitset<bits>(gen);
    benchmark::ClobberMemory();

    for(auto _: state)
    {
        benchmark::DoNotOptimize(bitset.count());
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(bits,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] =
      benchmark::Counter(bits, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

STD_BITSET_BENCHMARK_RANGE(std_bitset_count, "count");
