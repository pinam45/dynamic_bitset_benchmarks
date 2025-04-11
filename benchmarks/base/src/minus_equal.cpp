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
void sul_dynamic_bitset_minus_equal(benchmark::State& state)
{
    const size_t bits = static_cast<size_t>(state.range(0));
    std::minstd_rand gen(SEED);
    sul::dynamic_bitset<block_type_t> bitset1 = random_bitset<sul::dynamic_bitset<block_type_t>>(gen, bits);
    sul::dynamic_bitset<block_type_t> bitset2 = random_bitset<sul::dynamic_bitset<block_type_t>>(gen, bits);
    benchmark::ClobberMemory();

    for(auto _: state)
    {
        benchmark::DoNotOptimize(bitset1 -= bitset2);
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(bits,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] =
      benchmark::Counter(bits, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

SUL_DYNAMIC_BITSET_BENCHMARK_RANGE(sul_dynamic_bitset_minus_equal, "minus_equal");

#ifdef HAS_BOOST
template<typename block_type_t>
void boost_dynamic_bitset_minus_equal(benchmark::State& state)
{
    const size_t bits = static_cast<size_t>(state.range(0));
    std::minstd_rand gen(SEED);
    boost::dynamic_bitset<block_type_t> bitset1 = random_bitset<boost::dynamic_bitset<block_type_t>>(gen, bits);
    boost::dynamic_bitset<block_type_t> bitset2 = random_bitset<boost::dynamic_bitset<block_type_t>>(gen, bits);
    benchmark::ClobberMemory();

    for(auto _: state)
    {
        benchmark::DoNotOptimize(bitset1 -= bitset2);
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(bits,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] =
      benchmark::Counter(bits, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

BOOST_DYNAMIC_BITSET_BENCHMARK_RANGE(boost_dynamic_bitset_minus_equal, "minus_equal");
#endif

#ifdef HAS_STD_TR2_DYNAMIC_BITSET
template<typename block_type_t>
void std_tr2_dynamic_bitset_minus_equal(benchmark::State& state)
{
    const size_t bits = static_cast<size_t>(state.range(0));
    std::minstd_rand gen(SEED);
    std::tr2::dynamic_bitset<block_type_t> bitset1 = random_bitset<std::tr2::dynamic_bitset<block_type_t>>(gen, bits);
    std::tr2::dynamic_bitset<block_type_t> bitset2 = random_bitset<std::tr2::dynamic_bitset<block_type_t>>(gen, bits);
    benchmark::ClobberMemory();

    for(auto _: state)
    {
        benchmark::DoNotOptimize(bitset1 -= bitset2);
        benchmark::ClobberMemory();
    }

    state.counters["1_bit_time"] =
      benchmark::Counter(bits,
                         benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert,
                         benchmark::Counter::OneK::kIs1000);
    state.counters["bits_per_second"] =
      benchmark::Counter(bits, benchmark::Counter::kIsIterationInvariantRate, benchmark::Counter::OneK::kIs1024);
}

STD_TR2_DYNAMIC_BITSET_BENCHMARK_RANGE(std_tr2_dynamic_bitset_minus_equal, "minus_equal");
#endif

void std_vector_bool_minus_equal(benchmark::State& state)
{
    const size_t bits = static_cast<size_t>(state.range(0));
    std::minstd_rand gen(SEED);
    std::vector<bool> bitset1 = random_bitset<std::vector<bool>>(gen, bits);
    std::vector<bool> bitset2 = random_bitset<std::vector<bool>>(gen, bits);
    benchmark::ClobberMemory();

    for(auto _: state)
    {
        for(size_t i = 0; i < bits; ++i)
        {
            benchmark::DoNotOptimize(bitset1[i] = bitset1[i] - bitset2[i]);
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

STD_VECTOR_BOOL_BENCHMARK_RANGE(std_vector_bool_minus_equal, "minus_equal");
