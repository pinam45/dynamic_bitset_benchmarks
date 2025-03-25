//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include <uscp/instance.hpp>
#include <uscp/random.hpp>
#include <uscp/rwls.hpp>
#include <uscp/solution.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <deque>
#include <utility>
#include <vector>

namespace uscp::rwls
{
    constexpr size_t TABU_LIST_LENGTH = 2;

    template<typename dynamic_bitset_t>
    class rwls final
    {
    public:
        explicit rwls(const problem::instance<dynamic_bitset_t>& problem) noexcept;
        rwls(const rwls&) = default;
        rwls(rwls&&) noexcept = default;
        rwls& operator=(const rwls& other) = delete;
        rwls& operator=(rwls&& other) noexcept = delete;

        void initialize() noexcept;
        [[nodiscard]] solution<dynamic_bitset_t>
        improve(const solution<dynamic_bitset_t>& solution, random_engine& generator, size_t steps) noexcept;

    private:
        struct point_information final // row
        {
            long long weight = 1;
            size_t subsets_covering_in_solution = 0;
        };

        struct subset_information final // column
        {
            long long score = 0;
            long long timestamp = 1;
            bool canAddToSolution = true;
        };

        struct resolution_data final
        {
            random_engine& generator;
            solution<dynamic_bitset_t>& best_solution;
            solution<dynamic_bitset_t> current_solution;
            dynamic_bitset_t uncovered_points; // RWLS name: L
            std::vector<point_information> points_information;
            std::vector<subset_information> subsets_information;
            std::deque<size_t> tabu_subsets;

            dynamic_bitset_t subsets_tmp;

            explicit resolution_data(solution<dynamic_bitset_t>& solution, random_engine& generator) noexcept;
        };

        long long compute_subset_score(const resolution_data& data, size_t subset_number) noexcept;
        void init(resolution_data& data, const std::vector<long long>& points_weights_initial) noexcept;

        void add_subset(resolution_data& data, size_t subset_number) noexcept;
        void remove_subset(resolution_data& data, size_t subset_number) noexcept;

        void make_tabu(resolution_data& data, size_t subset_number) noexcept;
        [[nodiscard]] bool is_tabu(const resolution_data& data, size_t subset_number) noexcept;

        [[nodiscard]] static size_t select_subset_to_remove_no_timestamp(const resolution_data& data) noexcept;
        [[nodiscard]] size_t select_subset_to_remove(const resolution_data& data) noexcept;
        [[nodiscard]] size_t select_subset_to_add(const resolution_data& data, size_t point_to_cover) noexcept;
        [[nodiscard]] static size_t select_uncovered_point(resolution_data& data) noexcept;

        const uscp::problem::instance<dynamic_bitset_t>& m_problem;
        std::vector<std::vector<size_t>> m_subsets_points;
        std::vector<std::vector<size_t>> m_subsets_covering_points;
        bool m_initialized;
    };

    template<typename dynamic_bitset_t>
    [[nodiscard]] solution<dynamic_bitset_t>
    improve(const uscp::solution<dynamic_bitset_t>& solution_initial, random_engine& generator, size_t steps);
} // namespace uscp::rwls

template<typename dynamic_bitset_t>
uscp::rwls::rwls<dynamic_bitset_t>::rwls(const problem::instance<dynamic_bitset_t>& problem) noexcept
    : m_problem(problem)
    , m_subsets_points()
    , m_subsets_covering_points()
    , m_initialized(false)
{
    m_subsets_points.resize(m_problem.subsets_number);
    m_subsets_covering_points.resize(m_problem.points_number);
}

template<typename dynamic_bitset_t>
void uscp::rwls::rwls<dynamic_bitset_t>::initialize() noexcept
{
    for(size_t i = 0; i < m_problem.subsets_number; ++i)
    {
        fix::dynamic_bitset::do_iterate_bits_on(m_problem.subsets_points[i],
                                                [&](size_t bit_on) noexcept
                                                {
                                                    m_subsets_points[i].push_back(bit_on);
                                                    m_subsets_covering_points[bit_on].push_back(i);
                                                });
    }

    m_initialized = true;
}

template<typename dynamic_bitset_t>
uscp::solution<dynamic_bitset_t> uscp::rwls::rwls<dynamic_bitset_t>::improve(
  const uscp::solution<dynamic_bitset_t>& solution_initial, uscp::random_engine& generator, size_t steps) noexcept
{
    if(!m_initialized)
    {
        initialize();
    }

    solution<dynamic_bitset_t> solution_final = solution_initial;
    resolution_data data(solution_final, generator);
    const std::vector<long long> points_initial_weights(m_problem.points_number, 1);
    init(data, points_initial_weights);

    for(size_t step = 0; step < steps; ++step)
    {
        if(fix::dynamic_bitset::do_none(data.uncovered_points))
        {
            do
            {
                data.best_solution = data.current_solution;
                const size_t selected_subset = select_subset_to_remove_no_timestamp(data);
                remove_subset(data, selected_subset);
            } while(fix::dynamic_bitset::do_none(data.uncovered_points));

            data.best_solution.compute_cover();
            assert(data.best_solution.cover_all_points);
            if(!data.best_solution.cover_all_points)
            {
                abort();
            }
        }

        // remove subset
        const size_t subset_to_remove = select_subset_to_remove(data);
        remove_subset(data, subset_to_remove);
        data.subsets_information[subset_to_remove].timestamp = static_cast<long long>(step);

        // add subset
        const size_t selected_point = select_uncovered_point(data);
        const size_t subset_to_add = select_subset_to_add(data, selected_point);
        add_subset(data, subset_to_add);

        data.subsets_information[subset_to_add].timestamp = static_cast<long long>(step);
        make_tabu(data, subset_to_add);

        // update points weights
        fix::dynamic_bitset::do_iterate_bits_on(
          data.uncovered_points,
          [&](size_t uncovered_points_bit_on) noexcept
          {
              assert(data.points_information[uncovered_points_bit_on].subsets_covering_in_solution == 0);

              ++data.points_information[uncovered_points_bit_on].weight;

              // update subsets score depending on this point weight
              // subset that can cover the point if added to solution
              for(size_t subset_covering_point: m_subsets_covering_points[uncovered_points_bit_on])
              {
                  ++data.subsets_information[subset_covering_point].score;
              }
          });
#if !defined(NDEBUG)
        for(size_t i = 0; i < m_problem.subsets_number; ++i)
        {
            assert(data.subsets_information[i].score == compute_subset_score(data, i));
        }
#endif

        ++step;
    }

    return solution_final;
}

template<typename dynamic_bitset_t>
uscp::rwls::rwls<dynamic_bitset_t>::resolution_data::resolution_data(solution<dynamic_bitset_t>& solution,
                                                                     uscp::random_engine& generator_) noexcept
    : generator(generator_)
    , best_solution(solution)
    , current_solution(solution)
    , uncovered_points(solution.problem.points_number)
    , points_information()
    , subsets_information()
    , tabu_subsets()
    , subsets_tmp(solution.problem.subsets_number)
{
    points_information.resize(solution.problem.points_number);
    subsets_information.resize(solution.problem.subsets_number);
}

template<typename dynamic_bitset_t>
long long uscp::rwls::rwls<dynamic_bitset_t>::compute_subset_score(const resolution_data& data,
                                                                   size_t subset_number) noexcept
{
    assert(subset_number < m_problem.subsets_number);

    long long subset_score = 0;
    if(data.current_solution.selected_subsets[subset_number])
    {
        // if in solution, gain score for points covered only by the subset
        fix::dynamic_bitset::do_iterate_bits_on(m_problem.subsets_points[subset_number],
                                                [&](size_t bit_on) noexcept
                                                {
                                                    if(data.points_information[bit_on].subsets_covering_in_solution
                                                       == 1)
                                                    {
                                                        assert(!data.uncovered_points[bit_on]);
                                                        subset_score -= data.points_information[bit_on].weight;
                                                    }
                                                });
        assert(subset_score <= 0);
    }
    else
    {
        // if out of solution, gain score for uncovered points it can cover
        fix::dynamic_bitset::do_iterate_bits_on(m_problem.subsets_points[subset_number],
                                                [&](size_t bit_on) noexcept
                                                {
                                                    if(data.points_information[bit_on].subsets_covering_in_solution
                                                       == 0)
                                                    {
                                                        assert(data.uncovered_points[bit_on]);
                                                        subset_score += data.points_information[bit_on].weight;
                                                    }
                                                    else
                                                    {
                                                        assert(!data.uncovered_points[bit_on]);
                                                    }
                                                });
        assert(subset_score >= 0);
    }

    return subset_score;
}

template<typename dynamic_bitset_t>
void uscp::rwls::rwls<dynamic_bitset_t>::init(resolution_data& data,
                                              const std::vector<long long>& points_weights_initial) noexcept
{
    assert(points_weights_initial.size() == m_problem.points_number);

    // points information
    for(size_t i = 0; i < m_problem.points_number; ++i)
    {
        data.points_information[i].weight = points_weights_initial[i];
        data.points_information[i].subsets_covering_in_solution = 0;
        for(size_t subset_covering_point: m_subsets_covering_points[i])
        {
            if(fix::dynamic_bitset::do_test(data.current_solution.selected_subsets, subset_covering_point))
            {
                ++data.points_information[i].subsets_covering_in_solution;
            }
        }
    }

    // subset scores
    for(size_t i = 0; i < m_problem.subsets_number; ++i)
    {
        data.subsets_information[i].score = compute_subset_score(data, i);
        assert(data.current_solution.selected_subsets[i] ? data.subsets_information[i].score <= 0
                                                         : data.subsets_information[i].score >= 0);
    }
}

template<typename dynamic_bitset_t>
void uscp::rwls::rwls<dynamic_bitset_t>::add_subset(resolution_data& data, size_t subset_number) noexcept
{
    assert(subset_number < m_problem.subsets_number);
    assert(!data.current_solution.selected_subsets[subset_number]);
    assert(data.subsets_information[subset_number].score >= 0);

    // add subset to solution
    fix::dynamic_bitset::do_set(data.current_solution.selected_subsets, subset_number);
    fix::dynamic_bitset::do_minus_equal(data.uncovered_points, m_problem.subsets_points[subset_number]);

    // compute new score
    const long long new_score = -data.subsets_information[subset_number].score;

    // update subsets and points information
    for(size_t subset_point: m_subsets_points[subset_number])
    {
        ++data.points_information[subset_point].subsets_covering_in_solution;
        if(data.points_information[subset_point].subsets_covering_in_solution == 1)
        {
            // point newly covered
            const long long point_weight = data.points_information[subset_point].weight;
            for(size_t neighbor: m_subsets_covering_points[subset_point])
            {
                data.subsets_information[neighbor].canAddToSolution = true;

                // lost score because this point is now covered in the solution
                data.subsets_information[neighbor].score -= point_weight;
            }
        }
        else if(data.points_information[subset_point].subsets_covering_in_solution == 2)
        {
            // point now covered twice
            const long long point_weight = data.points_information[subset_point].weight;
            for(size_t neighbor: m_subsets_covering_points[subset_point])
            {
                data.subsets_information[neighbor].canAddToSolution = true;

                if(neighbor != subset_number
                   && fix::dynamic_bitset::do_test(data.current_solution.selected_subsets, neighbor))
                {
                    // lost score because it is no longer the only one to cover this point
                    data.subsets_information[neighbor].score += point_weight;
                    break;
                }
            }
        }
    }

    // update subset information
    data.subsets_information[subset_number].score = new_score;
    assert(data.subsets_information[subset_number].score == compute_subset_score(data, subset_number));

#if !defined(NDEBUG)
    for(size_t i = 0; i < m_problem.subsets_number; ++i)
    {
        assert(data.subsets_information[i].score == compute_subset_score(data, i));
    }
#endif
}

template<typename dynamic_bitset_t>
void uscp::rwls::rwls<dynamic_bitset_t>::remove_subset(resolution_data& data, size_t subset_number) noexcept
{
    assert(subset_number < m_problem.subsets_number);
    assert(data.current_solution.selected_subsets[subset_number]);
    assert(data.subsets_information[subset_number].score <= 0);

    // remove subset from solution
    fix::dynamic_bitset::do_reset(data.current_solution.selected_subsets, subset_number);

    // compute new score
    const long long new_score = -data.subsets_information[subset_number].score;

    // update subsets and points information
    for(size_t subset_point: m_subsets_points[subset_number])
    {
        --data.points_information[subset_point].subsets_covering_in_solution;
        if(data.points_information[subset_point].subsets_covering_in_solution == 0)
        {
            // point newly uncovered
            fix::dynamic_bitset::do_set(data.uncovered_points, subset_point);
            const long long point_weight = data.points_information[subset_point].weight;
            for(size_t neighbor: m_subsets_covering_points[subset_point])
            {
                data.subsets_information[neighbor].canAddToSolution = true;

                // gain score because this point is now uncovered in the solution
                data.subsets_information[neighbor].score += point_weight;
            }
        }
        else if(data.points_information[subset_point].subsets_covering_in_solution == 1)
        {
            // point now covered once
            const long long point_weight = data.points_information[subset_point].weight;
            for(size_t neighbor: m_subsets_covering_points[subset_point])
            {
                data.subsets_information[neighbor].canAddToSolution = true;

                if(neighbor != subset_number
                   && fix::dynamic_bitset::do_test(data.current_solution.selected_subsets, neighbor))
                {
                    // gain score because it is now the only one to cover this point in the solution
                    data.subsets_information[neighbor].score -= point_weight;
                    break;
                }
            }
        }
    }

    // update subset information
    data.subsets_information[subset_number].score = new_score;
    assert(data.subsets_information[subset_number].score == compute_subset_score(data, subset_number));
    data.subsets_information[subset_number].canAddToSolution = false;

#if !defined(NDEBUG)
    for(size_t i = 0; i < m_problem.subsets_number; ++i)
    {
        assert(data.subsets_information[i].score == compute_subset_score(data, i));
    }
#endif
}

template<typename dynamic_bitset_t>
void uscp::rwls::rwls<dynamic_bitset_t>::make_tabu(resolution_data& data, size_t subset_number) noexcept
{
    assert(subset_number < m_problem.subsets_number);
    data.tabu_subsets.push_back(subset_number);
    if(data.tabu_subsets.size() > TABU_LIST_LENGTH)
    {
        data.tabu_subsets.pop_front();
    }
}

template<typename dynamic_bitset_t>
bool uscp::rwls::rwls<dynamic_bitset_t>::is_tabu(const resolution_data& data, size_t subset_number) noexcept
{
    assert(subset_number < m_problem.subsets_number);
    return std::find(std::cbegin(data.tabu_subsets), std::cend(data.tabu_subsets), subset_number)
           != std::cend(data.tabu_subsets);
}

template<typename dynamic_bitset_t>
size_t uscp::rwls::rwls<dynamic_bitset_t>::select_subset_to_remove_no_timestamp(const resolution_data& data) noexcept
{
    assert(fix::dynamic_bitset::do_any(data.current_solution.selected_subsets));
    size_t selected_subset = fix::dynamic_bitset::do_find_first(data.current_solution.selected_subsets);
    assert(selected_subset < data.current_solution.selected_subsets.size());
    long long best_score = data.subsets_information[selected_subset].score;
    fix::dynamic_bitset::do_iterate_bits_on(data.current_solution.selected_subsets,
                                            [&](size_t bit_on) noexcept
                                            {
                                                if(data.subsets_information[bit_on].score > best_score)
                                                {
                                                    best_score = data.subsets_information[bit_on].score;
                                                    selected_subset = bit_on;
                                                }
                                            });
    assert(fix::dynamic_bitset::do_test(data.current_solution.selected_subsets, selected_subset));
    return selected_subset;
}

template<typename dynamic_bitset_t>
size_t uscp::rwls::rwls<dynamic_bitset_t>::select_subset_to_remove(const resolution_data& data) noexcept
{
    assert(fix::dynamic_bitset::do_any(data.current_solution.selected_subsets));
    size_t remove_subset = fix::dynamic_bitset::do_find_first(data.current_solution.selected_subsets);
    assert(remove_subset < data.current_solution.selected_subsets.size());
    std::pair<long long, long long> best_score_minus_timestamp(data.subsets_information[remove_subset].score,
                                                               -data.subsets_information[remove_subset].timestamp);
    fix::dynamic_bitset::do_iterate_bits_on(
      data.current_solution.selected_subsets,
      [&](size_t bit_on) noexcept
      {
          const std::pair<long long, long long> current_score_timestamp(data.subsets_information[bit_on].score,
                                                                        -data.subsets_information[bit_on].timestamp);
          if(current_score_timestamp > best_score_minus_timestamp && !is_tabu(data, bit_on))
          {
              best_score_minus_timestamp = current_score_timestamp;
              remove_subset = bit_on;
          }
      });
    assert(fix::dynamic_bitset::do_test(data.current_solution.selected_subsets, remove_subset));
    return remove_subset;
}

template<typename dynamic_bitset_t>
size_t uscp::rwls::rwls<dynamic_bitset_t>::select_subset_to_add(const resolution_data& data,
                                                                size_t point_to_cover) noexcept
{
    assert(point_to_cover < m_problem.points_number);
    assert(fix::dynamic_bitset::do_test(data.uncovered_points, point_to_cover));

    size_t add_subset = 0;
    bool add_subset_is_tabu = true;
    std::pair<long long, long long> best_score_minus_timestamp(std::numeric_limits<long long>::min(),
                                                               std::numeric_limits<long long>::max());
    bool found = false;
    for(size_t subset_covering: m_subsets_covering_points[point_to_cover])
    {
        if(fix::dynamic_bitset::do_test(data.current_solution.selected_subsets, subset_covering))
        {
            continue;
        }
        if(!data.subsets_information[subset_covering].canAddToSolution)
        {
            continue;
        }

        const std::pair<long long, long long> current_score_minus_timestamp(
          data.subsets_information[subset_covering].score, -data.subsets_information[subset_covering].timestamp);
        if(add_subset_is_tabu)
        {
            best_score_minus_timestamp = current_score_minus_timestamp;
            add_subset = subset_covering;
            add_subset_is_tabu = is_tabu(data, add_subset);
            found = true;
            continue;
        }
        if(current_score_minus_timestamp > best_score_minus_timestamp && !is_tabu(data, subset_covering))
        {
            best_score_minus_timestamp = current_score_minus_timestamp;
            add_subset = subset_covering;
        }
    }
    assert(found);

    assert(!fix::dynamic_bitset::do_test(data.current_solution.selected_subsets, add_subset));
    return add_subset;
}

template<typename dynamic_bitset_t>
size_t uscp::rwls::rwls<dynamic_bitset_t>::select_uncovered_point(resolution_data& data) noexcept
{
    assert(fix::dynamic_bitset::do_count(data.uncovered_points) > 0);
    size_t selected_point = 0;
    std::uniform_int_distribution<size_t> uncovered_point_dist(0, fix::dynamic_bitset::do_count(data.uncovered_points));
    const size_t selected_point_number = uncovered_point_dist(data.generator);
    size_t current_point_number = 0;
    fix::dynamic_bitset::do_iterate_bits_on(data.uncovered_points,
                                            [&](size_t bit_on) noexcept
                                            {
                                                if(++current_point_number >= selected_point_number)
                                                {
                                                    selected_point = bit_on;
                                                    return false;
                                                }
                                                return true;
                                            });
    assert(fix::dynamic_bitset::do_test(data.uncovered_points, selected_point));
    return selected_point;
}

template<typename dynamic_bitset_t>
uscp::solution<dynamic_bitset_t>
uscp::rwls::improve(const uscp::solution<dynamic_bitset_t>& solution_initial, random_engine& generator, size_t steps)
{
    rwls<dynamic_bitset_t> rwls(solution_initial.problem);
    rwls.initialize();
    return rwls.improve(solution_initial, generator, steps);
}
