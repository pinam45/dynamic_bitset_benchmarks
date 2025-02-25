//
// Copyright (c) 2025 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include <config.hpp>

#include <benchmark/benchmark.h>
#include <fmt/core.h>
#include <fmt/std.h>

#include <filesystem>

int main(int argc, char** argv)
{
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

    // Run benchmarks
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();

    return 0;
}
