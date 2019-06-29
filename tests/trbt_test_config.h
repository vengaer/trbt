#ifndef TRBT_TEST_CONFIG_H
#define TRBT_TEST_CONFIG_H

#define TRBT_TEST_FLAG inline bool constexpr

#pragma once
#include <cstddef>

#define TRACE_STREAM std::cout

namespace trbt {
namespace test {

/* int */
TRBT_TEST_FLAG test_int_insert                    = false;
TRBT_TEST_FLAG test_int_insert_range              = false;
TRBT_TEST_FLAG test_int_hinted_insert             = true;
TRBT_TEST_FLAG test_int_emplace                   = false;
TRBT_TEST_FLAG test_int_hinted_emplace            = true;
TRBT_TEST_FLAG test_int_erase                     = false;

/* std::string */
TRBT_TEST_FLAG test_string_insert                 = false;
TRBT_TEST_FLAG test_string_insert_range           = false;
TRBT_TEST_FLAG test_string_hinted_insert          = true;
TRBT_TEST_FLAG test_string_emplace                = false;
TRBT_TEST_FLAG test_string_hinted_emplace         = true;
TRBT_TEST_FLAG test_string_erase                  = false;

/* std::pair<int, double> */
TRBT_TEST_FLAG test_pair_insert                   = false;
TRBT_TEST_FLAG test_pair_insert_range             = false;
TRBT_TEST_FLAG test_pair_hinted_insert            = true;
TRBT_TEST_FLAG test_pair_emplace                  = false;
TRBT_TEST_FLAG test_pair_hinted_emplace           = true;
TRBT_TEST_FLAG test_pair_erase                    = false;

/* std::pair<int, double>, piecewise */
TRBT_TEST_FLAG test_pair_piecewise_emplace        = false;
TRBT_TEST_FLAG test_pair_piecewise_hinted_emplace = false;

inline std::size_t constexpr trbt_trace_size           = 3u;

} /* namespace test */
} /* namespace trbt */

#undef TRBT_TEST_FLAG
#endif
