//
// Copyright (c) 2025 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include <benchmark/benchmark.h>

#include <chrono>
#include <thread>

static void BM_SomeFunction(benchmark::State& state)
{
    for(auto _: state)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main(int argc, char** argv)
{
    benchmark::RegisterBenchmark("BM_SomeFunction", BM_SomeFunction);
    benchmark::Initialize(&argc, argv);
    // TODO: use unrecognized arguments remaining in argc/argv
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
}
