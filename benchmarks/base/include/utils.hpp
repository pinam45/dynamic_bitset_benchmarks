//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include <climits>
#include <cstddef>
#include <type_traits>

template<typename T>
constexpr size_t bits_number = sizeof(T) * CHAR_BIT;

template<typename T>
constexpr bool bit_value(T value, size_t bit_pos)
{
    static_assert(std::is_unsigned<T>::value, "T is not an unsigned integral type");
    assert(bit_pos < bits_number<T>);
    return (value & (T(1) << bit_pos)) != T(0);
}

template<typename dynamic_bitset_t>
dynamic_bitset_t random_bitset(std::minstd_rand& gen, size_t bits)
{
    std::bernoulli_distribution d;
    dynamic_bitset_t bitset;
    if constexpr(requires { dynamic_bitset_t::reserve(bits); })
    {
        bitset.reserve(bits);
    }
    for(size_t i = 0; i < bits; ++i)
    {
        bitset.push_back(d(gen));
    }
    return bitset;
}
