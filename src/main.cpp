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
		benchmark::DoNotOptimize(dynamic_bitset<T>());
	}
}
BENCHMARK_TEMPLATE(constructor_default, uint8_t);
BENCHMARK_TEMPLATE(constructor_default, uint16_t);
BENCHMARK_TEMPLATE(constructor_default, uint32_t);
BENCHMARK_TEMPLATE(constructor_default, uint64_t);

template<typename T>
void constructor_nbits_initval(benchmark::State& state)
{
	std::mt19937 gen(SEED);
	std::uniform_int_distribution<unsigned long long> d_value;
	std::uniform_int_distribution<size_t> d_bits_to_take(1, bits_number<unsigned long long>);

	for(auto _: state)
	{
		benchmark::DoNotOptimize(dynamic_bitset<T>(d_bits_to_take(gen), d_value(gen)));
	}
}
BENCHMARK_TEMPLATE(constructor_nbits_initval, uint8_t);
BENCHMARK_TEMPLATE(constructor_nbits_initval, uint16_t);
BENCHMARK_TEMPLATE(constructor_nbits_initval, uint32_t);
BENCHMARK_TEMPLATE(constructor_nbits_initval, uint64_t);

BENCHMARK_MAIN();
