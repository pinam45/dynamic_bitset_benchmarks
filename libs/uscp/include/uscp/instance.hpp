//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include <cstddef>
#include <filesystem>
#include <string>
#include <vector>

namespace uscp::problem
{
    template<typename dynamic_bitset_t>
    struct instance final
    {
        std::string name;
        size_t points_number = 0;
        size_t subsets_number = 0;
        std::vector<dynamic_bitset_t> subsets_points;

        instance() noexcept = default;
        instance(const instance&) = default;
        instance(instance&&) noexcept = default;
        instance& operator=(const instance&) = default;
        instance& operator=(instance&&) noexcept = default;
    };
} // namespace uscp::problem
