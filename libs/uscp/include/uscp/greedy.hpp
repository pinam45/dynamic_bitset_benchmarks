//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include <uscp/greedy.hpp>
#include <uscp/solution.hpp>

#include <cstdlib>

namespace uscp::greedy
{
    template<typename dynamic_bitset_t>
    [[nodiscard]] solution<dynamic_bitset_t> solve(const problem::instance<dynamic_bitset_t>& problem) noexcept;
} // namespace uscp::greedy

template<typename dynamic_bitset_t>
[[nodiscard]] uscp::solution<dynamic_bitset_t> uscp::greedy::solve(const uscp::problem::instance<dynamic_bitset_t>& problem) noexcept
{
    uscp::solution<dynamic_bitset_t> solution(problem);

    while(!solution.cover_all_points)
    {
        size_t max_subset_number = solution.selected_subsets.size(); // invalid initial value
        dynamic_bitset_t covered_points_with_max_subset(problem.points_number);
        size_t covered_points_number_with_max_subset = solution.covered_points.count();
        dynamic_bitset_t new_covered_points;
        for(size_t i = 0; i < problem.subsets_number; ++i)
        {
            if(solution.selected_subsets[i])
            {
                // already selected
                continue;
            }

            new_covered_points = solution.covered_points;
            new_covered_points |= problem.subsets_points[i];
            const size_t new_covered_points_number = new_covered_points.count();
            if(new_covered_points_number > covered_points_number_with_max_subset)
            {
                max_subset_number = i;
                covered_points_with_max_subset = new_covered_points;
                covered_points_number_with_max_subset = new_covered_points_number;
            }
        }

        // all subset already included or no subset add covered points
        if(max_subset_number == solution.selected_subsets.size())
        {
            std::abort();
        }

        // update solution
        solution.selected_subsets.set(max_subset_number);
        solution.covered_points = covered_points_with_max_subset;
        solution.cover_all_points = solution.covered_points.all();
    }

    solution.compute_cover();
    assert(solution.cover_all_points);

    return solution;
}
