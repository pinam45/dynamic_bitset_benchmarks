//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include "utils.hpp"

#include <benchmark/benchmark.h>
#include <dynamic_bitset.hpp>

#include <cstring>
#include <random>

static constexpr std::minstd_rand::result_type SEED = 314159;
//const std::minstd_rand::result_type SEED = std::random_device{}();
static constexpr size_t RANGE_START = 1ull << 2u;
static constexpr size_t RANGE_END = 1ull << 30u;
static constexpr size_t RANGE_MULTIPLIER = 1ull << 2u;
#define RANGE_BENCHMARK_TEMPLATE(n, ...)  \
	BENCHMARK_TEMPLATE(n, __VA_ARGS__)    \
	  ->RangeMultiplier(RANGE_MULTIPLIER) \
	  ->Range(RANGE_START, RANGE_END)
#define DYNAMIC_BITSET_RANGE_BENCHMARK(n, b)  \
	RANGE_BENCHMARK_TEMPLATE(n, b<uint16_t>); \
	RANGE_BENCHMARK_TEMPLATE(n, b<uint32_t>); \
	RANGE_BENCHMARK_TEMPLATE(n, b<uint64_t>)
#define DYNAMIC_BITSET_BENCHMARK(n, b)  \
	BENCHMARK_TEMPLATE(n, b<uint16_t>); \
	BENCHMARK_TEMPLATE(n, b<uint32_t>); \
	BENCHMARK_TEMPLATE(n, b<uint64_t>)

template<typename dynamic_bitset_t>
void constructor_default(benchmark::State& state)
{
	for(auto _: state)
	{
		benchmark::DoNotOptimize(dynamic_bitset_t());
	}
}
DYNAMIC_BITSET_BENCHMARK(constructor_default, sul::dynamic_bitset);

template<typename dynamic_bitset_t>
void constructor_nbits_initval(benchmark::State& state)
{
	std::minstd_rand gen(SEED);
	std::uniform_int_distribution<size_t> d_bits_to_take(1, bits_number<unsigned long long>);
	std::uniform_int_distribution<unsigned long long> d_value;

	for(auto _: state)
	{
		const size_t bits_to_take = d_bits_to_take(gen);
		const unsigned long long value = d_value(gen);

		benchmark::DoNotOptimize(dynamic_bitset_t(bits_to_take, value));
	}
}
DYNAMIC_BITSET_BENCHMARK(constructor_nbits_initval, sul::dynamic_bitset);

template<typename dynamic_bitset_t>
void constructor_initializer_list_one_value(benchmark::State& state)
{
	using block_type = typename dynamic_bitset_t::block_type;
	std::minstd_rand gen(SEED);
	std::uniform_int_distribution<block_type> d_value;

	for(auto _: state)
	{
		const block_type value = d_value(gen);

		benchmark::DoNotOptimize(dynamic_bitset_t{value});
	}
}
DYNAMIC_BITSET_BENCHMARK(constructor_initializer_list_one_value, sul::dynamic_bitset);

template<typename dynamic_bitset_t>
void constructor_initializer_list_two_value(benchmark::State& state)
{
	using block_type = typename dynamic_bitset_t::block_type;
	std::minstd_rand gen(SEED);
	std::uniform_int_distribution<block_type> d_value;

	for(auto _: state)
	{
		const block_type value1 = d_value(gen);
		const block_type value2 = d_value(gen);

		benchmark::DoNotOptimize(dynamic_bitset_t{value1, value2});
	}
}
DYNAMIC_BITSET_BENCHMARK(constructor_initializer_list_two_value, sul::dynamic_bitset);

template<typename dynamic_bitset_t>
void constructor_string(benchmark::State& state)
{
	const size_t string_size = static_cast<size_t>(state.range(0));
	std::minstd_rand gen(SEED);
	std::bernoulli_distribution d_bit;
	std::string str;
	str.reserve(string_size);
	for(size_t i = 0; i < string_size; ++i)
	{
		str.push_back(d_bit(gen) ? '1' : '0');
	}

	for(auto _: state)
	{
		benchmark::DoNotOptimize(dynamic_bitset_t(str));
		benchmark::ClobberMemory();
	}

	state.counters["1_char_time"] = benchmark::Counter(string_size,
	                                                   benchmark::Counter::kIsIterationInvariantRate
	                                                     | benchmark::Counter::kInvert,
	                                                   benchmark::Counter::OneK::kIs1000);
	state.counters["chars_per_second"] =
	  benchmark::Counter(string_size,
	                     benchmark::Counter::kIsIterationInvariantRate,
	                     benchmark::Counter::OneK::kIs1024);
}
DYNAMIC_BITSET_RANGE_BENCHMARK(constructor_string, sul::dynamic_bitset);

BENCHMARK_MAIN();
