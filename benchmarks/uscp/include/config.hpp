//
// Copyright (c) 2025 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include <random>

const std::minstd_rand::result_type SEED = std::random_device{}();
