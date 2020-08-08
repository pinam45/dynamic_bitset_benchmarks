//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include <cstring>
#include <random>

#include <benchmark/benchmark.h>
#include <dynamic_bitset.hpp>

#include "utils.hpp"

constexpr std::minstd_rand::result_type SEED = 314159;
//const std::minstd_rand::result_type SEED = std::random_device{}();

template<typename T>
void constructor_default(benchmark::State& state)
{
	for(auto _: state)
	{
		benchmark::DoNotOptimize(sul::dynamic_bitset<T>());
	}
}
BENCHMARK_TEMPLATE(constructor_default, uint16_t);
BENCHMARK_TEMPLATE(constructor_default, uint32_t);
BENCHMARK_TEMPLATE(constructor_default, uint64_t);

template<typename T>
void constructor_nbits_initval(benchmark::State& state)
{
	std::mt19937 gen(SEED);
	std::uniform_int_distribution<size_t> d_bits_to_take(1, bits_number<unsigned long long>);
	std::uniform_int_distribution<unsigned long long> d_value;

	for(auto _: state)
	{
		const size_t bits_to_take = d_bits_to_take(gen);
		const unsigned long long value = d_value(gen);

		benchmark::DoNotOptimize(sul::dynamic_bitset<T>(bits_to_take, value));
	}
}
BENCHMARK_TEMPLATE(constructor_nbits_initval, uint16_t);
BENCHMARK_TEMPLATE(constructor_nbits_initval, uint32_t);
BENCHMARK_TEMPLATE(constructor_nbits_initval, uint64_t);

template<typename T>
void constructor_initializer_list_one_value(benchmark::State& state)
{
	std::mt19937 gen(SEED);
	std::uniform_int_distribution<T> d_value;

	for(auto _: state)
	{
		const T value = d_value(gen);

		benchmark::DoNotOptimize(sul::dynamic_bitset<T>{value});
	}
}
BENCHMARK_TEMPLATE(constructor_initializer_list_one_value, uint16_t);
BENCHMARK_TEMPLATE(constructor_initializer_list_one_value, uint32_t);
BENCHMARK_TEMPLATE(constructor_initializer_list_one_value, uint64_t);

template<typename T>
void constructor_initializer_list_two_value(benchmark::State& state)
{
	std::mt19937 gen(SEED);
	std::uniform_int_distribution<T> d_value;

	for(auto _: state)
	{
		const T value1 = d_value(gen);
		const T value2 = d_value(gen);

		benchmark::DoNotOptimize(sul::dynamic_bitset<T>{value1, value2});
	}
}
BENCHMARK_TEMPLATE(constructor_initializer_list_two_value, uint16_t);
BENCHMARK_TEMPLATE(constructor_initializer_list_two_value, uint32_t);
BENCHMARK_TEMPLATE(constructor_initializer_list_two_value, uint64_t);

BENCHMARK_MAIN();
