#ifndef TRBT_TEST_CONFIG_H
#define TRBT_TEST_CONFIG_H

#define TRBT_TEST_FLAG inline bool constexpr

#pragma once
#include <cstddef>
#include <iostream>

/* TODO
 * find
 * swap
 * lower_bound
 * upper_bound
 * iterators
 * operator==
 * operator!=
 * operator<
 * operator>
 * operator<=
 * operator>=
 */

namespace trbt {
namespace test {

inline std::size_t constexpr trbt_trace_size      = 1u;
inline std::ostream& trbt_trace_stream            = std::cout;

/* int */
TRBT_TEST_FLAG test_int_copy_ctor                 = false;
TRBT_TEST_FLAG test_int_move_ctor                 = false;
TRBT_TEST_FLAG test_int_copy_assignment           = false;
TRBT_TEST_FLAG test_int_move_assignment           = false;
TRBT_TEST_FLAG test_int_empty                     = false;
TRBT_TEST_FLAG test_int_size                      = false;
TRBT_TEST_FLAG test_int_clear                     = false;
TRBT_TEST_FLAG test_int_contains                  = false;
TRBT_TEST_FLAG test_int_count                     = true;
TRBT_TEST_FLAG test_int_insert                    = false;
TRBT_TEST_FLAG test_int_insert_range              = false;
TRBT_TEST_FLAG test_int_hinted_insert             = false;
TRBT_TEST_FLAG test_int_emplace                   = false;
TRBT_TEST_FLAG test_int_hinted_emplace            = false;
TRBT_TEST_FLAG test_int_erase                     = false;

/* std::string */
TRBT_TEST_FLAG test_string_copy_ctor              = false;
TRBT_TEST_FLAG test_string_move_ctor              = false;
TRBT_TEST_FLAG test_string_copy_assignment        = false;
TRBT_TEST_FLAG test_string_move_assignment        = false;
TRBT_TEST_FLAG test_string_empty                  = false;
TRBT_TEST_FLAG test_string_size                   = false;
TRBT_TEST_FLAG test_string_clear                  = false;
TRBT_TEST_FLAG test_string_contains               = false;
TRBT_TEST_FLAG test_string_count                  = true;
TRBT_TEST_FLAG test_string_insert                 = false;
TRBT_TEST_FLAG test_string_insert_range           = false;
TRBT_TEST_FLAG test_string_hinted_insert          = false;
TRBT_TEST_FLAG test_string_emplace                = false;
TRBT_TEST_FLAG test_string_hinted_emplace         = false;
TRBT_TEST_FLAG test_string_erase                  = false;

/* std::pair<int, double> */
TRBT_TEST_FLAG test_pair_copy_ctor                = false;
TRBT_TEST_FLAG test_pair_move_ctor                = false;
TRBT_TEST_FLAG test_pair_copy_assignment          = false;
TRBT_TEST_FLAG test_pair_move_assignment          = false;
TRBT_TEST_FLAG test_pair_empty                    = false;
TRBT_TEST_FLAG test_pair_size                     = false;
TRBT_TEST_FLAG test_pair_clear                    = false;
TRBT_TEST_FLAG test_pair_contains                 = false;
TRBT_TEST_FLAG test_pair_count                    = true;
TRBT_TEST_FLAG test_pair_insert                   = false;
TRBT_TEST_FLAG test_pair_insert_range             = false;
TRBT_TEST_FLAG test_pair_hinted_insert            = false;
TRBT_TEST_FLAG test_pair_emplace                  = false;
TRBT_TEST_FLAG test_pair_hinted_emplace           = false;
TRBT_TEST_FLAG test_pair_erase                    = false;

/* std::pair<int, double>, piecewise */
TRBT_TEST_FLAG test_pair_piecewise_emplace        = false;
TRBT_TEST_FLAG test_pair_piecewise_hinted_emplace = false;

} /* namespace test */
} /* namespace trbt */

#undef TRBT_TEST_FLAG
#endif
