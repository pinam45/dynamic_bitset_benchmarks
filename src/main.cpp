//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include <benchmark/benchmark.h>
#include <dynamic_bitset.hpp>

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

BENCHMARK_MAIN();
