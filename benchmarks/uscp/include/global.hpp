//
// Copyright (c) 2025 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include <sul/dynamic_bitset.hpp>
#ifdef HAS_BOOST
#    include <boost/dynamic_bitset.hpp>
#endif
#ifdef HAS_STD_TR2_DYNAMIC_BITSET
#    include <tr2/dynamic_bitset>
#endif

#include <uscp/instance.hpp>

namespace global
{
    template<typename dynamic_bitset_t>
    extern uscp::problem::instance<dynamic_bitset_t> benchmark_instance;
} // namespace global

#ifndef GLOBAL_BENCHMARK_INSTANCE_DEFINITION
extern template uscp::problem::instance<sul::dynamic_bitset<uint16_t>>
  global::benchmark_instance<sul::dynamic_bitset<uint16_t>>;
extern template uscp::problem::instance<sul::dynamic_bitset<uint32_t>>
  global::benchmark_instance<sul::dynamic_bitset<uint32_t>>;
extern template uscp::problem::instance<sul::dynamic_bitset<uint64_t>>
  global::benchmark_instance<sul::dynamic_bitset<uint64_t>>;
#    ifdef HAS_BOOST
extern template uscp::problem::instance<boost::dynamic_bitset<uint16_t>>
  global::benchmark_instance<boost::dynamic_bitset<uint16_t>>;
extern template uscp::problem::instance<boost::dynamic_bitset<uint32_t>>
  global::benchmark_instance<boost::dynamic_bitset<uint32_t>>;
extern template uscp::problem::instance<boost::dynamic_bitset<uint64_t>>
  global::benchmark_instance<boost::dynamic_bitset<uint64_t>>;
#    endif
#    ifdef HAS_STD_TR2_DYNAMIC_BITSET
extern template uscp::problem::instance<std::tr2::dynamic_bitset<uint16_t>>
  global::benchmark_instance<std::tr2::dynamic_bitset<uint16_t>>;
extern template uscp::problem::instance<std::tr2::dynamic_bitset<uint32_t>>
  global::benchmark_instance<std::tr2::dynamic_bitset<uint32_t>>;
extern template uscp::problem::instance<std::tr2::dynamic_bitset<uint64_t>>
  global::benchmark_instance<std::tr2::dynamic_bitset<uint64_t>>;
#    endif
#endif
