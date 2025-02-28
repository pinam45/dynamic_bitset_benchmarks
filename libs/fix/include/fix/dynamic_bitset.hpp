//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include <algorithm>
#include <ranges>
#include <vector>

namespace fix::dynamic_bitset
{
    template<typename T>
    struct dependent_false : public std::false_type
    {
    };

    template<typename dynamic_bitset_t>
    [[nodiscard]] constexpr size_t do_count(const dynamic_bitset_t& bitset) noexcept
    {
        // if std::vector<bool>
        if constexpr(std::is_same_v<dynamic_bitset_t, std::vector<bool>>)
        {
            return std::count(bitset.cbegin(), bitset.cend(), true);
        }
        // if sane dynamic_bitset
        else
        {
            return bitset.count();
        }
    }

    template<typename dynamic_bitset_t>
    constexpr void do_or_equal(dynamic_bitset_t& lhs, const dynamic_bitset_t& rhs) noexcept
    {
        // if std::vector<bool>
        if constexpr(std::is_same_v<dynamic_bitset_t, std::vector<bool>>)
        {
            // for(const size_t i: std::views::iota(0u, lhs.size()))
            // {
            //     lhs[i] = lhs[i] || rhs[i];
            // }
            std::transform(lhs.cbegin(), lhs.cend(), rhs.cbegin(), lhs.begin(), std::logical_or<bool>());
        }
        // if sane dynamic_bitset
        else
        {
            lhs |= rhs;
        }
    }

    template<typename dynamic_bitset_t>
    constexpr void do_set(dynamic_bitset_t& bitset, size_t pos, bool value = true) noexcept
    {
        // if std::vector<bool>
        if constexpr(std::is_same_v<dynamic_bitset_t, std::vector<bool>>)
        {
            bitset[pos] = value;
        }
        // if sane dynamic_bitset
        else
        {
            bitset.set(pos, value);
        }
    }

    template<typename dynamic_bitset_t>
    [[nodiscard]] constexpr bool do_all(const dynamic_bitset_t& bitset) noexcept
    {
        // if std::vector<bool>
        if constexpr(std::is_same_v<dynamic_bitset_t, std::vector<bool>>)
        {
            return std::ranges::all_of(bitset, [](bool val) noexcept { return val; });
        }
        // if sane dynamic_bitset
        else
        {
            return bitset.all();
        }
    }

    template<typename dynamic_bitset_t>
    constexpr void do_reset(dynamic_bitset_t& bitset) noexcept
    {
        // if std::vector<bool>
        if constexpr(std::is_same_v<dynamic_bitset_t, std::vector<bool>>)
        {
            // for(const size_t i: std::views::iota(0u, bitset.size()))
            // {
            //     bitset[i] = false;
            // }
            std::fill(bitset.begin(), bitset.end(), false);
        }
        // if sane dynamic_bitset
        else
        {
            bitset.reset();
        }
    }

} // namespace fix::dynamic_bitset
