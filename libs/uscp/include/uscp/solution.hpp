//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include <uscp/instance.hpp>

#include <utility>

namespace uscp
{
    template<typename dynamic_bitset_t>
    struct solution final
    {
        const problem::instance<dynamic_bitset_t>& problem;
        dynamic_bitset_t selected_subsets;
        dynamic_bitset_t covered_points;
        bool cover_all_points;

        explicit solution(const problem::instance<dynamic_bitset_t>& problem);
        solution(const solution&) = default;
        solution(solution&&) noexcept = default;
        solution& operator=(const solution& other);
        solution& operator=(solution&& other) noexcept;

        void compute_cover() noexcept;
    };
} // namespace uscp

template<typename dynamic_bitset_t>
uscp::solution<dynamic_bitset_t>::solution(const uscp::problem::instance<dynamic_bitset_t>& problem_)
    : problem(problem_)
    , selected_subsets(problem_.subsets_number)
    , covered_points(problem_.points_number)
    , cover_all_points(false)
{
}

template<typename dynamic_bitset_t>
uscp::solution<dynamic_bitset_t>& uscp::solution<dynamic_bitset_t>::operator=(const solution<dynamic_bitset_t>& other)
{
    assert(&problem == &other.problem);
    selected_subsets = other.selected_subsets;
    covered_points = other.covered_points;
    cover_all_points = other.cover_all_points;
    return *this;
}

template<typename dynamic_bitset_t>
uscp::solution<dynamic_bitset_t>&
uscp::solution<dynamic_bitset_t>::operator=(solution<dynamic_bitset_t>&& other) noexcept
{
    assert(&problem == &other.problem);
    selected_subsets = std::move(other.selected_subsets);
    covered_points = std::move(other.covered_points);
    cover_all_points = other.cover_all_points;
    return *this;
}

template<typename dynamic_bitset_t>
void uscp::solution<dynamic_bitset_t>::compute_cover() noexcept
{
    assert(selected_subsets.size() == problem.subsets_number);
    assert(covered_points.size() == problem.points_number);

    covered_points.reset();
    size_t selected_subset = selected_subsets.find_first();
    while(selected_subset != dynamic_bitset_t::npos)
    {
        covered_points |= problem.subsets_points[selected_subset];
        selected_subset = selected_subsets.find_next(selected_subset);
    }
    cover_all_points = covered_points.all();
}
