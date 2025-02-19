//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include <uscp/instance.hpp>

#include <fmt/format.h>
#include <fmt/std.h>
#include <tl/expected.hpp>

#include <filesystem>
#include <fstream>
#include <string>

namespace uscp::problem::or_library
{
    template<typename dynamic_bitset_t>
    tl::expected<instance<dynamic_bitset_t>, std::string> instance_from(const std::filesystem::path& path) noexcept
    {
        instance<dynamic_bitset_t> instance;
        instance.subsets_number = 0;
        instance.points_number = 0;
        instance.subsets_points.clear();

        // Check path exists
        if(std::error_code error; !std::filesystem::exists(path, error))
        {
            if(error)
            {
                return tl::unexpected(fmt::format("{}: std::filesystem::exists failed, {}", path, error.message()));
            }
            return tl::unexpected(fmt::format("{}: file does not exist", path));
        }

        // Check path is a regular file
        if(std::error_code error; !std::filesystem::is_regular_file(path, error))
        {
            if(error)
            {
                return tl::unexpected(
                  fmt::format("{}: std::filesystem::is_regular_file failed, {}", path, error.message()));
            }
            return tl::unexpected(fmt::format("{}: is not a regular file", path));
        }

        // Open instance stream
        std::ifstream instance_stream(path);
        if(!instance_stream)
        {
            return tl::unexpected(fmt::format("{}: failed to read file", path));
        }

        // Read points number
        if(!instance_stream.good())
        {
            return tl::unexpected(fmt::format("{}: invalid file format", path));
        }
        size_t points_number = 0;
        instance_stream >> points_number;
        if(points_number == 0)
        {
            return tl::unexpected(fmt::format("{}: invalid points number: {}", path, points_number));
        }
        instance.points_number = points_number;

        // Read subsets number
        if(!instance_stream.good())
        {
            return tl::unexpected(fmt::format("{}: invalid file format", path));
        }
        size_t subsets_number = 0;
        instance_stream >> subsets_number;
        if(subsets_number == 0)
        {
            return tl::unexpected(fmt::format("{}: invalid subsets number: {}", path, subsets_number));
        }
        instance.subsets_number = subsets_number;

        // Read subsets costs
        for(size_t i = 0; i < subsets_number; ++i)
        {
            if(!instance_stream.good())
            {
                return tl::unexpected(fmt::format("{}: invalid file format", path));
            }
            size_t ignored_subset_cost;
            instance_stream >> ignored_subset_cost;
        }

        // Read subsets covering points
        instance.subsets_points.resize(subsets_number);
        for(size_t i = 0; i < subsets_number; ++i)
        {
            instance.subsets_points[i].resize(points_number);
        }
        for(size_t i_point = 0; i_point < points_number; ++i_point)
        {
            size_t subsets_covering_point = 0;
            if(!instance_stream.good())
            {
                return tl::unexpected(fmt::format("{}: invalid file format", path));
            }
            instance_stream >> subsets_covering_point;
            if(subsets_covering_point > subsets_number)
            {
                return tl::unexpected(fmt::format(
                  "{}: invalid value, supposedly {} subsets covering point {}, but instance contains only {} subsets",
                  path,
                  subsets_covering_point,
                  i_point,
                  subsets_number));
            }
            for(size_t i_subset = 0; i_subset < subsets_covering_point; ++i_subset)
            {
                size_t subset_number = 0;
                if(!instance_stream.good())
                {
                    return tl::unexpected(fmt::format("{}: invalid file format", path));
                }
                instance_stream >> subset_number;
                if(subset_number == 0)
                {
                    return tl::unexpected(fmt::format(
                      "{}: invalid value, supposedly subset 0 cover point {}, but subsets are numbered from 1",
                      path,
                      i_point));
                }
                --subset_number; // numbered from 1 in the file
                if(subset_number >= subsets_number)
                {
                    return tl::unexpected(fmt::format(
                      "{}: invalid value, supposedly subset {} cover point {}, but instance contains only {} subsets",
                      path,
                      subset_number + 1,
                      i_point,
                      subsets_number));
                }
                instance.subsets_points[subset_number][i_point] = true;
            }
        }

        // Success
        return instance;
    }
} // namespace uscp::problem::or_library
