//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include <algorithm>
#include <concepts>
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
        if constexpr(std::same_as<std::remove_cvref_t<dynamic_bitset_t>, std::vector<bool>>)
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
        if constexpr(std::same_as<std::remove_cvref_t<dynamic_bitset_t>, std::vector<bool>>)
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
    constexpr void do_minus_equal(dynamic_bitset_t& lhs, const dynamic_bitset_t& rhs) noexcept
    {
        // if std::vector<bool>
        if constexpr(std::same_as<std::remove_cvref_t<dynamic_bitset_t>, std::vector<bool>>)
        {
            // for(const size_t i: std::views::iota(0u, lhs.size()))
            // {
            //     lhs[i] = lhs[i] - rhs[i];
            // }
            std::transform(lhs.cbegin(),
                           lhs.cend(),
                           rhs.cbegin(),
                           lhs.begin(),
                           [](bool lhs_value, bool rhs_value) noexcept { return lhs_value & !rhs_value; });
        }
        // if sane dynamic_bitset
        else
        {
            lhs -= rhs;
        }
    }

    template<typename dynamic_bitset_t>
    constexpr void do_set(dynamic_bitset_t& bitset, size_t pos, bool value = true) noexcept
    {
        // if std::vector<bool>
        if constexpr(std::same_as<std::remove_cvref_t<dynamic_bitset_t>, std::vector<bool>>)
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
    constexpr bool do_test(dynamic_bitset_t& bitset, size_t pos) noexcept
    {
        // if std::vector<bool>
        if constexpr(std::same_as<std::remove_cvref_t<dynamic_bitset_t>, std::vector<bool>>)
        {
            return bitset[pos];
        }
        // if sane dynamic_bitset
        else
        {
            return bitset.test(pos);
        }
    }

    template<typename dynamic_bitset_t>
    [[nodiscard]] constexpr bool do_all(const dynamic_bitset_t& bitset) noexcept
    {
        // if std::vector<bool>
        if constexpr(std::same_as<std::remove_cvref_t<dynamic_bitset_t>, std::vector<bool>>)
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
    [[nodiscard]] constexpr bool do_none(const dynamic_bitset_t& bitset) noexcept
    {
        // if std::vector<bool>
        if constexpr(std::same_as<std::remove_cvref_t<dynamic_bitset_t>, std::vector<bool>>)
        {
            return std::ranges::none_of(bitset, [](bool val) noexcept { return val; });
        }
        // if sane dynamic_bitset
        else
        {
            return bitset.none();
        }
    }

    template<typename dynamic_bitset_t>
    [[nodiscard]] constexpr bool do_any(const dynamic_bitset_t& bitset) noexcept
    {
        // if std::vector<bool>
        if constexpr(std::same_as<std::remove_cvref_t<dynamic_bitset_t>, std::vector<bool>>)
        {
            return std::ranges::any_of(bitset, [](bool val) noexcept { return val; });
        }
        // if sane dynamic_bitset
        else
        {
            return bitset.any();
        }
    }

    template<typename dynamic_bitset_t>
    constexpr void do_reset(dynamic_bitset_t& bitset) noexcept
    {
        // if std::vector<bool>
        if constexpr(std::same_as<std::remove_cvref_t<dynamic_bitset_t>, std::vector<bool>>)
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

    template<typename dynamic_bitset_t>
    constexpr void do_reset(dynamic_bitset_t& bitset, size_t pos) noexcept
    {
        // if std::vector<bool>
        if constexpr(std::same_as<std::remove_cvref_t<dynamic_bitset_t>, std::vector<bool>>)
        {
            bitset[pos] = false;
        }
        // if sane dynamic_bitset
        else
        {
            bitset.reset(pos);
        }
    }

    template<typename dynamic_bitset_t>
    [[nodiscard]] constexpr size_t do_find_first(const dynamic_bitset_t& bitset) noexcept
    {
        // if std::vector<bool>
        if constexpr(std::same_as<std::remove_cvref_t<dynamic_bitset_t>, std::vector<bool>>)
        {
            if(const auto it = std::find(bitset.cbegin(), bitset.cend(), true); it != bitset.cend())
            {
                return std::distance(bitset.cbegin(), it);
            }
            return std::numeric_limits<size_t>::max();
        }
        // if sane dynamic_bitset
        else
        {
            return bitset.find_first();
        }
    }

    template<typename dynamic_bitset_t, typename Function, typename... Parameters>
    constexpr void
    do_iterate_bits_on(dynamic_bitset_t& bitset, Function&& function, Parameters&&... parameters) noexcept
    {
        // check
        if constexpr(!std::is_invocable_v<Function, size_t, Parameters...>)
        {
            static_assert(dependent_false<Function>::value, "Function take invalid arguments");
            // function should take (size_t, parameters...) as arguments
        }

        // if std::vector<bool>
        if constexpr(std::same_as<std::remove_cvref_t<dynamic_bitset_t>, std::vector<bool>>)
        {
            if constexpr(std::same_as<std::invoke_result_t<Function, size_t, Parameters...>, void>)
            {
                for(const size_t i_bit: std::views::iota(0u, bitset.size()))
                {
                    if(bitset[i_bit])
                    {
                        std::invoke(std::forward<Function>(function), i_bit, std::forward<Parameters>(parameters)...);
                    }
                }
            }
            else if constexpr(std::is_convertible_v<std::invoke_result_t<Function, size_t, Parameters...>, bool>)
            {
                for(const size_t i_bit: std::views::iota(0u, bitset.size()))
                {
                    if(bitset[i_bit])
                    {
                        if(!std::invoke(
                             std::forward<Function>(function), i_bit, std::forward<Parameters>(parameters)...))
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                static_assert(dependent_false<Function>::value, "Function have invalid return type");
                // return type should be void, or convertible to bool
            }
        }
        // if dynamic_bitset without iterate_bits_on
        else if constexpr(!requires {
                              bitset.iterate_bits_on(std::forward<Function>(function),
                                                     std::forward<Parameters>(parameters)...);
                          })
        {
            if constexpr(std::same_as<std::invoke_result_t<Function, size_t, Parameters...>, void>)
            {
                size_t i_bit = bitset.find_first();
                // boost::dynamic_bitset::find_next() returns dynamic_bitset_t::npos
                // std::tr2::dynamic_bitset::find_next() returns dynamic_bitset.size()
                while(i_bit < bitset.size())
                {
                    std::invoke(std::forward<Function>(function), i_bit, std::forward<Parameters>(parameters)...);
                    i_bit = bitset.find_next(i_bit);
                }
            }
            else if constexpr(std::is_convertible_v<std::invoke_result_t<Function, size_t, Parameters...>, bool>)
            {
                size_t i_bit = bitset.find_first();
                // boost::dynamic_bitset::find_next() returns dynamic_bitset_t::npos
                // std::tr2::dynamic_bitset::find_next() returns dynamic_bitset.size()
                while(i_bit < bitset.size())
                {
                    if(!std::invoke(std::forward<Function>(function), i_bit, std::forward<Parameters>(parameters)...))
                    {
                        break;
                    }
                    i_bit = bitset.find_next(i_bit);
                }
            }
            else
            {
                static_assert(dependent_false<Function>::value, "Function have invalid return type");
                // return type should be void, or convertible to bool
            }
        }
        // if dynamic_bitset with iterate_bits_on
        else
        {
            bitset.iterate_bits_on(std::forward<Function>(function), std::forward<Parameters>(parameters)...);
        }
    }

} // namespace fix::dynamic_bitset
