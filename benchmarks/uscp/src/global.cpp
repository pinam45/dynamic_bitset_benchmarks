//
// Copyright (c) 2025 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#define GLOBAL_BENCHMARK_INSTANCE_DEFINITION
#include <global.hpp>

#include <sul/dynamic_bitset.hpp>
#ifdef HAS_BOOST
#    include <boost/dynamic_bitset.hpp>
#endif
#ifdef HAS_STD_TR2_DYNAMIC_BITSET
#    include <tr2/dynamic_bitset>
#endif

#include <vector>

template<>
uscp::problem::instance<sul::dynamic_bitset<uint16_t>> global::benchmark_instance<sul::dynamic_bitset<uint16_t>>;
template<>
uscp::problem::instance<sul::dynamic_bitset<uint32_t>> global::benchmark_instance<sul::dynamic_bitset<uint32_t>>;
template<>
uscp::problem::instance<sul::dynamic_bitset<uint64_t>> global::benchmark_instance<sul::dynamic_bitset<uint64_t>>;

#ifdef HAS_BOOST
template<>
uscp::problem::instance<boost::dynamic_bitset<uint16_t>> global::benchmark_instance<boost::dynamic_bitset<uint16_t>>;
template<>
uscp::problem::instance<boost::dynamic_bitset<uint32_t>> global::benchmark_instance<boost::dynamic_bitset<uint32_t>>;
template<>
uscp::problem::instance<boost::dynamic_bitset<uint64_t>> global::benchmark_instance<boost::dynamic_bitset<uint64_t>>;
#endif

#ifdef HAS_STD_TR2_DYNAMIC_BITSET
template<>
uscp::problem::instance<std::tr2::dynamic_bitset<uint16_t>>
  global::benchmark_instance<std::tr2::dynamic_bitset<uint16_t>>;
template<>
uscp::problem::instance<std::tr2::dynamic_bitset<uint32_t>>
  global::benchmark_instance<std::tr2::dynamic_bitset<uint32_t>>;
template<>
uscp::problem::instance<std::tr2::dynamic_bitset<uint64_t>>
  global::benchmark_instance<std::tr2::dynamic_bitset<uint64_t>>;
#endif

template<>
uscp::problem::instance<std::vector<bool>> global::benchmark_instance<std::vector<bool>>;
