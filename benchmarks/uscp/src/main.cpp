//
// Copyright (c) 2025 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include <config.hpp>
#include <global.hpp>
#include <greedy.hpp>

#include <uscp/or_library.hpp>

#include <sul/dynamic_bitset.hpp>
#ifdef HAS_BOOST
#    include <boost/dynamic_bitset.hpp>
#endif
#ifdef HAS_STD_TR2_DYNAMIC_BITSET
#    include <tr2/dynamic_bitset>
#endif

#include <benchmark/benchmark.h>
#include <fmt/core.h>
#include <fmt/std.h>

#include <chrono>
#include <filesystem>

bool load_instance(const std::filesystem::path& instance_path) noexcept
{
#define LOAD_INSTANCE_FOR(BITSET_TYPE) \
    if(auto instance = uscp::problem::or_library::instance_from<BITSET_TYPE>(instance_path)) \
    { \
        fmt::print(stderr, " [✓] " #BITSET_TYPE "\n"); \
        global::benchmark_instance<BITSET_TYPE> = *instance; \
    } \
    else \
    { \
        fmt::print(stderr, "failed load instance: {}\n", instance.error()); \
        return false; \
    }
    LOAD_INSTANCE_FOR(sul::dynamic_bitset<uint16_t>);
    LOAD_INSTANCE_FOR(sul::dynamic_bitset<uint32_t>);
    LOAD_INSTANCE_FOR(sul::dynamic_bitset<uint64_t>);
#ifdef HAS_BOOST
    LOAD_INSTANCE_FOR(boost::dynamic_bitset<uint16_t>);
    LOAD_INSTANCE_FOR(boost::dynamic_bitset<uint32_t>);
    LOAD_INSTANCE_FOR(boost::dynamic_bitset<uint64_t>);
#endif
#ifdef HAS_STD_TR2_DYNAMIC_BITSET
    LOAD_INSTANCE_FOR(std::tr2::dynamic_bitset<uint16_t>);
    LOAD_INSTANCE_FOR(std::tr2::dynamic_bitset<uint32_t>);
    LOAD_INSTANCE_FOR(std::tr2::dynamic_bitset<uint64_t>);
#endif
    LOAD_INSTANCE_FOR(std::vector<bool>);
#undef LOAD_INSTANCE_FOR

    return true;
}

int main(int argc, char** argv)
{
    // Register greedy benchmark for each dynamic bitset type
    SUL_DYNAMIC_BITSET_REGISTER_BENCHMARK(sul_dynamic_bitset_uscp_greedy, "greedy");
#ifdef HAS_BOOST
    BOOST_DYNAMIC_BITSET_REGISTER_BENCHMARK(boost_dynamic_bitset_uscp_greedy, "greedy");
#endif
#ifdef HAS_STD_TR2_DYNAMIC_BITSET
    STD_TR2_DYNAMIC_BITSET_REGISTER_BENCHMARK(std_tr2_dynamic_bitset_uscp_greedy, "greedy");
#endif
    STD_VECTOR_BOOL_REGISTER_BENCHMARK(std_vector_bool_uscp_greedy, "greedy");

    // Process arguments
    benchmark::Initialize(&argc, argv);
    if(argc != 2)
    {
        fmt::print(stderr, "invalid number of arguments remaining after google/benckmark: {}\n", argc - 1);
        return 1;
    }
    std::filesystem::path instance_path(argv[1]);

    // Check the instance file exists
    if(std::error_code exists_error; !std::filesystem::exists(instance_path, exists_error))
    {
        if(exists_error)
        {
            fmt::print(stderr, "filesystem error: {}\n", exists_error.message());
        }
        else
        {
            fmt::print(stderr, "file does not exist: {}\n", instance_path);
        }
        return 1;
    }

    // Check the instance file is a regular file
    if(std::error_code is_regular_file_error; !std::filesystem::is_regular_file(instance_path, is_regular_file_error))
    {
        if(is_regular_file_error)
        {
            fmt::print(stderr, "filesystem error: {}\n", is_regular_file_error.message());
        }
        else
        {
            fmt::print(stderr, "file is not a regular file: {}\n", instance_path);
        }
        return 1;
    }

    // Load the instance
    fmt::print(stderr, "load OR-Library instance from: {}\n", instance_path);
    if(!load_instance(instance_path))
    {
        return 1;
    }
    fmt::print(stderr,
               "instance loaded: {} points, {} subsets\n\n",
               global::benchmark_instance<sul::dynamic_bitset<uint16_t>>.points_number,
               global::benchmark_instance<sul::dynamic_bitset<uint16_t>>.subsets_number);

    // Run benchmarks
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();

    return 0;
}
