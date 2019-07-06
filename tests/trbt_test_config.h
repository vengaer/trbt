#ifndef TRBT_TEST_CONFIG_H
#define TRBT_TEST_CONFIG_H

#define TRBT_TEST_FLAG inline bool constexpr

#pragma once
#include <cstddef>
#include <iostream>

namespace trbt {
namespace test {

inline std::size_t constexpr trbt_trace_size      = 1u;
inline std::ostream& trbt_trace_stream            = std::cout;

/* int */
TRBT_TEST_FLAG test_int_copy_ctor                 = true;
TRBT_TEST_FLAG test_int_move_ctor                 = true;
TRBT_TEST_FLAG test_int_copy_assignment           = true;
TRBT_TEST_FLAG test_int_move_assignment           = true;
TRBT_TEST_FLAG test_int_empty                     = true;
TRBT_TEST_FLAG test_int_size                      = true;
TRBT_TEST_FLAG test_int_clear                     = true;
TRBT_TEST_FLAG test_int_contains                  = true;
TRBT_TEST_FLAG test_int_count                     = true;
TRBT_TEST_FLAG test_int_find                      = true;
TRBT_TEST_FLAG test_int_swap                      = true;
TRBT_TEST_FLAG test_int_lower_bound               = true;
TRBT_TEST_FLAG test_int_upper_bound               = true;
TRBT_TEST_FLAG test_int_insert                    = true;
TRBT_TEST_FLAG test_int_insert_range              = true;
TRBT_TEST_FLAG test_int_hinted_insert             = true;
TRBT_TEST_FLAG test_int_emplace                   = true;
TRBT_TEST_FLAG test_int_hinted_emplace            = true;
TRBT_TEST_FLAG test_int_erase                     = true;
TRBT_TEST_FLAG test_int_eq                        = true;
TRBT_TEST_FLAG test_int_neq                       = true;
TRBT_TEST_FLAG test_int_less                      = true;
TRBT_TEST_FLAG test_int_greater                   = true;
TRBT_TEST_FLAG test_int_less_or_eq                = true;
TRBT_TEST_FLAG test_int_greater_or_eq             = true;
TRBT_TEST_FLAG test_int_iters                     = true;

/* std::string */
TRBT_TEST_FLAG test_string_copy_ctor              = true;
TRBT_TEST_FLAG test_string_move_ctor              = true;
TRBT_TEST_FLAG test_string_copy_assignment        = true;
TRBT_TEST_FLAG test_string_move_assignment        = true;
TRBT_TEST_FLAG test_string_empty                  = true;
TRBT_TEST_FLAG test_string_size                   = true;
TRBT_TEST_FLAG test_string_clear                  = true;
TRBT_TEST_FLAG test_string_contains               = true;
TRBT_TEST_FLAG test_string_count                  = true;
TRBT_TEST_FLAG test_string_find                   = true;
TRBT_TEST_FLAG test_string_swap                   = true;
TRBT_TEST_FLAG test_string_lower_bound            = true;
TRBT_TEST_FLAG test_string_upper_bound            = true;
TRBT_TEST_FLAG test_string_insert                 = true;
TRBT_TEST_FLAG test_string_insert_range           = true;
TRBT_TEST_FLAG test_string_hinted_insert          = true;
TRBT_TEST_FLAG test_string_emplace                = true;
TRBT_TEST_FLAG test_string_hinted_emplace         = true;
TRBT_TEST_FLAG test_string_erase                  = true;
TRBT_TEST_FLAG test_string_eq                     = true;
TRBT_TEST_FLAG test_string_neq                    = true;
TRBT_TEST_FLAG test_string_greater                = true;
TRBT_TEST_FLAG test_string_less                   = true;
TRBT_TEST_FLAG test_string_greater_or_eq          = true;
TRBT_TEST_FLAG test_string_less_or_eq             = true;
TRBT_TEST_FLAG test_string_iters                  = true;

/* std::pair<int, double> */
TRBT_TEST_FLAG test_pair_copy_ctor                = true;
TRBT_TEST_FLAG test_pair_move_ctor                = true;
TRBT_TEST_FLAG test_pair_copy_assignment          = true;
TRBT_TEST_FLAG test_pair_move_assignment          = true;
TRBT_TEST_FLAG test_pair_empty                    = true;
TRBT_TEST_FLAG test_pair_size                     = true;
TRBT_TEST_FLAG test_pair_clear                    = true;
TRBT_TEST_FLAG test_pair_contains                 = true;
TRBT_TEST_FLAG test_pair_count                    = true;
TRBT_TEST_FLAG test_pair_find                     = true;
TRBT_TEST_FLAG test_pair_swap                     = true;
TRBT_TEST_FLAG test_pair_lower_bound              = true;
TRBT_TEST_FLAG test_pair_upper_bound              = true;
TRBT_TEST_FLAG test_pair_insert                   = true;
TRBT_TEST_FLAG test_pair_insert_range             = true;
TRBT_TEST_FLAG test_pair_hinted_insert            = true;
TRBT_TEST_FLAG test_pair_emplace                  = true;
TRBT_TEST_FLAG test_pair_hinted_emplace           = true;
TRBT_TEST_FLAG test_pair_erase                    = true;
TRBT_TEST_FLAG test_pair_eq                       = true;
TRBT_TEST_FLAG test_pair_neq                      = true;
TRBT_TEST_FLAG test_pair_greater                  = true;
TRBT_TEST_FLAG test_pair_less                     = true;
TRBT_TEST_FLAG test_pair_greater_or_eq            = true;
TRBT_TEST_FLAG test_pair_less_or_eq               = true;
TRBT_TEST_FLAG test_pair_iters                    = true;

/* std::pair<int, double>, piecewise */
TRBT_TEST_FLAG test_pair_piecewise_emplace        = true;
TRBT_TEST_FLAG test_pair_piecewise_hinted_emplace = true;

} /* namespace test */
} /* namespace trbt */

#undef TRBT_TEST_FLAG
#endif
