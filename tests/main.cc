#include "trbt_test_config.h"
#include "trbt_test_framework.h"
#include "trbt_trace_type.h"
#include <random>
#include <sstream>
#include <utility>

int main() {
    using namespace trbt;
    std::mt19937 mt{std::random_device{}()};
    std::uniform_int_distribution<> iter_dis(0, 1000);
    std::uniform_int_distribution<> test_size_dis(1, 1500);
    
    test::trbt_trace_type<rbtree<int>> int_tree;
    test::trbt_trace_type<rbtree<std::string>> str_tree;
    test::trbt_trace_type<rbtree<std::pair<int, double>>> pair_tree;

    std::size_t total_iters = 0u;
    int iters;
    
    try {
        /* ------------------ */
        /* Copy ctor test int */
        /* ------------------ */
        if constexpr(test::test_int_copy_ctor) {
            impl::scoped_bool sb{int_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("COPY CTOR (int)", test_size, i, iters);
                auto vec = test::generate_int_vec(test_size);
                int_tree.insert(std::begin(vec), std::end(vec));
        
                test::copy_ctor(int_tree, [](int i) {
                    return std::to_string(i);
                });
            }
        }

        /* ------------------ */
        /* Move ctor test int */
        /* ------------------ */
        if constexpr(test::test_int_move_ctor) {
            impl::scoped_bool sb{int_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("MOVE CTOR (int)", test_size, i ,iters);
                auto vec = test::generate_int_vec(test_size);
                int_tree.insert(std::begin(vec), std::end(vec));
        
                test::move_ctor(int_tree, [](int i) {
                    return std::to_string(i);
                });
            }
        }

        /* ------------------------ */
        /* Copy assignment test int */
        /* ------------------------ */
        if constexpr(test::test_int_copy_assignment) {
            impl::scoped_bool sb{int_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;

            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("COPY ASSIGNMENT (int)", test_size, i, iters);
                auto vec = test::generate_int_vec(test_size);
                int_tree.insert(std::begin(vec), std::end(vec));
        
                test::copy_assignment(int_tree, [](int i) {
                    return std::to_string(i);
                });
            }
        }

        /* ------------------------ */
        /* Move assignment test int */
        /* ------------------------ */
        if constexpr(test::test_int_move_assignment) {
            impl::scoped_bool sb{int_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;

            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("MOVE ASSIGNMENT (int)", test_size, i, iters);
                auto vec = test::generate_int_vec(test_size);
                int_tree.insert(std::begin(vec), std::end(vec));
        
                test::move_assignment(int_tree, [](int i) {
                    return std::to_string(i);
                });
            }
        }

        /* -------------- */
        /* Empty test int */
        /* -------------- */
        if constexpr(test::test_int_empty) {
            impl::scoped_bool(int_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            
            for(int i = 0; i < iters; i++) {
                test::print_heading("EMPTY (int)", 1, i, iters);
                test::empty(int_tree);
            }
        }

        /* ------------- */
        /* Size test int */
        /* ------------- */
        if constexpr(test::test_int_size) {
            impl::scoped_bool(int_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("SIZE (int)", test_size, i, iters);
                auto vec = test::generate_int_vec(test_size);
                test::size(int_tree, vec);
            }
        }

        /* -------------- */
        /* Clear test int */
        /* -------------- */
        if constexpr(test::test_int_clear) {
            impl::scoped_bool(int_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;

            for(int i = 0; i < iters; i++) {
                test::print_heading("CLEAR (int)", 1, i, iters);
                test::clear(int_tree);
            }
        }

        /* ----------------- */
        /* Contains test int */
        /* ----------------- */
        if constexpr(test::test_int_contains) {
            impl::scoped_bool(int_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                auto vec = test::generate_int_vec(test_size);
                test::print_heading("CONTAINS (int)", test_size, i, iters);
                int_tree.insert(std::begin(vec), std::end(vec));
                test::contains(int_tree, vec, [](int i) {
                    return std::to_string(i);
                });
            }
        }

        /* -------------- */
        /* Count test int */
        /* -------------- */
        if constexpr(test::test_int_count) {
            impl::scoped_bool(int_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;

            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                auto vec = test::generate_int_vec(test_size);
                test::print_heading("COUNT (int)", test_size, i, iters);
                int_tree.insert(std::begin(vec), std::end(vec));
                test::count(int_tree, vec, [](int i) {
                    return std::to_string(i);
                });
            }
        }

        /* ------------- */
        /* Find test int */
        /* ------------- */
        if constexpr(test::test_int_find) {
            impl::scoped_bool(int_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                auto vec = test::generate_int_vec(test_size);
                test::print_heading("FIND (int)", test_size, i, iters);
                test::find(int_tree, vec, [](int i) {
                    return std::to_string(i);
                });
            }
        }

        /* ------------- */
        /* Swap test int */
        /* ------------- */
        if constexpr(test::test_int_swap) {
            impl::scoped_bool(int_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                auto vec = test::generate_int_vec(test_size);
                test::print_heading("SWAP (int)", test_size, i, iters);
                int_tree.clear();
                int_tree.insert(std::begin(vec), std::end(vec));

                test::trbt_trace_type<rbtree<int>> tree;
                test_size = test_size_dis(mt);
                vec = test::generate_int_vec(test_size);
                tree.insert(std::begin(vec), std::end(vec));

                test::swap(int_tree, tree, [](int i) {
                    return std::to_string(i);
                });
            }
        }

        /* -------------------- */
        /* lower_bound test int */
        /* -------------------- */
        if constexpr(test::test_int_lower_bound) {
            impl::scoped_bool(int_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                auto vec = test::generate_int_vec(test_size);
                test::print_heading("LOWER BOUND (int)", test_size, i, iters);
                int_tree.insert(std::begin(vec), std::end(vec));
                vec = test::generate_int_vec(test_size);
                test::lower_bound(int_tree, vec);
            }
        }

        /* -------------------- */
        /* upper_bound test int */
        /* -------------------- */
        if constexpr(test::test_int_upper_bound) {
            impl::scoped_bool(int_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                auto vec = test::generate_int_vec(test_size);
                test::print_heading("UPPER BOUND (int)", test_size, i, iters);
                int_tree.insert(std::begin(vec), std::end(vec));
                vec = test::generate_int_vec(test_size);
                test::upper_bound(int_tree, vec);
            }
        }

        /* --------------- */
        /* Insert test int */
        /* --------------- */
        if constexpr(test::test_int_insert) {
            impl::scoped_bool sb{int_tree.active};

            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("INSERT (int)", test_size, i, iters); 
                
                auto vec = test::generate_int_vec(test_size);
                test::insert(int_tree, vec, [](int i) {
                    return std::to_string(i);
                });

                test::contains(int_tree, vec, [](int i) {
                    return std::to_string(i);
                });
            }
        }

        /* ---------------------- */
        /* Hinted insert test int */
        /* ---------------------- */
        if constexpr(test::test_int_hinted_insert) {
            impl::scoped_bool sb{int_tree.active};

            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("HINTED INSERT (int)", test_size, i, iters); 
                
                auto vec = test::generate_int_vec(test_size);
                test::hinted_insert(int_tree, vec, [](int i) {
                    return std::to_string(i);
                });

                test::contains(int_tree, vec, [](int i) {
                    return std::to_string(i);
                });
            }
        }

        /* --------------------- */
        /* Insert range test int */
        /* --------------------- */
        if constexpr(test::test_int_insert_range) {
            impl::scoped_bool sb{int_tree.active};

            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("INSERT RANGE (int)", test_size, i, iters); 
                
                auto vec = test::generate_int_vec(test_size);
                test::insert_range(int_tree, vec, [](int i) {
                    return std::to_string(i);
                });

                test::contains(int_tree, vec, [](int i) {
                    return std::to_string(i);
                });
            }
        }

        /* ---------------- */
        /* Emplace test int */
        /* ---------------- */
        if constexpr(test::test_int_emplace) {
            impl::scoped_bool sb{int_tree.active};

            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("EMPLACE (int)", test_size, i, iters); 
                
                auto vec = test::generate_int_vec(test_size);
                test::emplace(int_tree, vec, [](int i) {
                    return std::to_string(i);
                });

                test::contains(int_tree, vec, [](int i) {
                    return std::to_string(i);
                });
            }
        }

        /* ----------------------- */
        /* Hinted emplace test int */
        /* ----------------------- */
        if constexpr(test::test_int_hinted_emplace) {
            impl::scoped_bool sb{int_tree.active};

            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("HINTED EMPLACE (int)", test_size, i, iters); 
                
                auto vec = test::generate_int_vec(test_size);
                test::hinted_emplace(int_tree, vec, [](int i) {
                    return std::to_string(i);
                });

                test::contains(int_tree, vec, [](int i) {
                    return std::to_string(i);
                });
            }
        }

        /* -------------- */
        /* Erase test int */
        /* -------------- */
        if constexpr(test::test_int_erase) {
            impl::scoped_bool sb{int_tree.active};

            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("ERASE (int)", test_size, i, iters);
                
                auto vec = test::generate_int_vec(test_size);

                int_tree.insert(std::begin(vec), std::end(vec));

                test::erase(int_tree, vec, [](int i) {
                    return std::to_string(i);
                });

                test::empty(int_tree);
            }
        }
        
        /* ------------------- */
        /* operator== test int */
        /* ------------------- */
        if constexpr(test::test_int_eq) {
            impl::scoped_bool sb{int_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("OPERATOR== (int)", test_size, i, iters);

                auto vec = test::generate_int_vec(test_size);
                int_tree.clear();
                int_tree.insert(std::begin(vec), std::end(vec));

                test::eq(int_tree, vec);
            }
        }

        /* ------------------- */
        /* operator!= test int */
        /* ------------------- */
        if constexpr(test::test_int_neq) {
            impl::scoped_bool sb{int_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("OPERATOR!= (int)", test_size, i, iters);

                auto vec = test::generate_int_vec(test_size);
                int_tree.clear();
                int_tree.insert(std::begin(vec), std::end(vec));

                test::neq(int_tree, vec);
            }
        }

        /* ------------------ */
        /* operator< test int */
        /* ------------------ */
        if constexpr(test::test_int_less) {
            impl::scoped_bool sb{int_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;

            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("OPERATOR< (int)", test_size, i, iters);

                auto vec = test::generate_int_vec(test_size);
                test::less(int_tree, vec, [](int i) {
                    return ++i;
                });
            }
        }

        /* ------------------ */
        /* operator> test int */
        /* ------------------ */
        if constexpr(test::test_int_greater) {
            impl::scoped_bool sb{int_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;

            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("OPERATOR> (int)", test_size, i, iters);

                auto vec = test::generate_int_vec(test_size);
                test::greater(int_tree, vec, [](int i) {
                    return ++i;
                });
            }
        }

        /* ------------------- */
        /* operator<= test int */
        /* ------------------- */
        if constexpr(test::test_int_less_or_eq) {
            impl::scoped_bool sb{int_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;

            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("OPERATOR<= (int)", test_size, i, iters);

                auto vec = test::generate_int_vec(test_size);
                test::less_or_eq(int_tree, vec, [](int i) {
                    return ++i;
                });
            }
        }

        /* ------------------- */
        /* operator>= test int */
        /* ------------------- */
        if constexpr(test::test_int_greater_or_eq) {
            impl::scoped_bool sb{int_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;

            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("OPERATOR>= (int)", test_size, i, iters);

                auto vec = test::generate_int_vec(test_size);
                test::greater_or_eq(int_tree, vec, [](int i) {
                    return ++i;
                });
            }
        }

        /* ----------------- */
        /* Iterator test int */
        /* ----------------- */
        if constexpr(test::test_int_iters) {
            impl::scoped_bool sb{int_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("ITERS (int)", test_size, i, iters);
                auto vec = test::generate_int_vec(test_size);
                test::iters(int_tree, vec);
            }
        }

        /* -------------------------- */
        /* Copy ctor test std::string */
        /* -------------------------- */
        if constexpr(test::test_string_copy_ctor) {
            impl::scoped_bool sb{str_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("COPY CTOR (std::string)", test_size, i, iters);
                auto vec = test::generate_string_vec(test_size);
                str_tree.insert(std::begin(vec), std::end(vec));
        
                test::copy_ctor(str_tree, [](auto const& str) {
                    return str;
                });
            }
        }

        /* -------------------------- */
        /* Move ctor test std::string */
        /* -------------------------- */
        if constexpr(test::test_string_move_ctor) {
            impl::scoped_bool sb{str_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("MOVE CTOR (std::string)", test_size, i, iters);
                auto vec = test::generate_string_vec(test_size);
                str_tree.insert(std::begin(vec), std::end(vec));
        
                test::move_ctor(str_tree, [](auto const& str) {
                    return str;
                });
            }
        }

        /* -------------------------------- */
        /* Copy assignment test std::string */
        /* -------------------------------- */
        if constexpr(test::test_string_copy_assignment) {
            impl::scoped_bool sb{str_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("COPY ASSIGNMENT (std::string)", test_size, i, iters);
                auto vec = test::generate_string_vec(test_size);
                str_tree.insert(std::begin(vec), std::end(vec));
        
                test::copy_assignment(str_tree, [](auto const& str) {
                    return str;
                });
            }
        }

        /* -------------------------- */
        /* Move assignment test std::string */
        /* -------------------------- */
        if constexpr(test::test_string_move_assignment) {
            impl::scoped_bool sb{str_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("MOVE ASSIGNMENT (std::string)", test_size, i, iters);
                auto vec = test::generate_string_vec(test_size);
                str_tree.insert(std::begin(vec), std::end(vec));
        
                test::move_assignment(str_tree, [](auto const& str) {
                    return str;
                });
            }
        }

        /* ---------------------- */
        /* Empty test std::string */
        /* ---------------------- */
        if constexpr(test::test_string_empty) {
            impl::scoped_bool(str_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                test::print_heading("EMPTY (std::string)", 1, i, iters);
                test::empty(str_tree);
            }
        }

        /* --------------------- */
        /* Size test std::string */
        /* --------------------- */
        if constexpr(test::test_string_size) {
            impl::scoped_bool(str_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                auto vec = test::generate_string_vec(test_size);
                test::print_heading("SIZE (std::string)", test_size, i, iters);
                test::size(str_tree, vec);
            }
        }

        /* ---------------------- */
        /* Clear test std::string */
        /* ---------------------- */
        if constexpr(test::test_string_clear) {
            impl::scoped_bool(str_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                test::print_heading("CLEAR (std::string)", 1, i, iters);
                test::clear(str_tree);
            }
        }

        /* ------------------------- */
        /* Contains test std::string */
        /* ------------------------- */
        if constexpr(test::test_string_contains) {
            impl::scoped_bool(str_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                auto vec = test::generate_string_vec(test_size);
                test::print_heading("CONTAINS (std::string)", test_size, i, iters);
                str_tree.insert(std::begin(vec), std::end(vec));
                test::contains(str_tree, vec, [](auto const& str) {
                    return str;
                });
            }
        }

        /* ---------------------- */
        /* Count test std::string */
        /* ---------------------- */
        if constexpr(test::test_string_count) {
            impl::scoped_bool(str_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                auto vec = test::generate_string_vec(test_size);
                test::print_heading("COUNT (std::string)", test_size, i, iters);
                str_tree.insert(std::begin(vec), std::end(vec));
                test::count(str_tree, vec, [](auto const& str) {
                    return str;
                });
            }
        }

        /* --------------------- */
        /* Find test std::string */
        /* --------------------- */
        if constexpr(test::test_string_find) {
            impl::scoped_bool(str_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                auto vec = test::generate_string_vec(test_size);
                test::print_heading("FIND (std::string)", test_size, i, iters);
                test::find(str_tree, vec, [](auto const& str) {
                    return str;
                });
            }
        }

        /* --------------------- */
        /* Swap test std::string */
        /* --------------------- */
        if constexpr(test::test_string_swap) {
            impl::scoped_bool(str_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                auto vec = test::generate_string_vec(test_size);
                test::print_heading("SWAP (std::string)", test_size, i, iters);
                str_tree.clear();
                str_tree.insert(std::begin(vec), std::end(vec));

                test::trbt_trace_type<rbtree<std::string>> tree;
                test_size = test_size_dis(mt);
                vec = test::generate_string_vec(test_size);
                tree.insert(std::begin(vec), std::end(vec));

                test::swap(str_tree, tree, [](auto const& str) {
                    return str;
                });
            }
        }

        /* ---------------------------- */
        /* lower_bound test std::string */
        /* ---------------------------- */
        if constexpr(test::test_string_lower_bound) {
            impl::scoped_bool(str_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                auto vec = test::generate_string_vec(test_size);
                test::print_heading("LOWER BOUND (std::string)", test_size, i, iters);
                str_tree.insert(std::begin(vec), std::end(vec));
                vec = test::generate_string_vec(test_size);
                test::lower_bound(str_tree, vec);
            }
        }

        /* ---------------------------- */
        /* upper_bound test std::string */
        /* ---------------------------- */
        if constexpr(test::test_string_upper_bound) {
            impl::scoped_bool(str_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                auto vec = test::generate_string_vec(test_size);
                test::print_heading("UPPER BOUND (std::string)", test_size, i, iters);
                str_tree.insert(std::begin(vec), std::end(vec));
                vec = test::generate_string_vec(test_size);
                test::upper_bound(str_tree, vec);
            }
        }

        /* ----------------------- */
        /* Insert test std::string */
        /* ----------------------- */
        if constexpr(test::test_string_insert) {
            impl::scoped_bool sb{str_tree.active};

            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("INSERT (std::string)", test_size, i, iters); 
                
                auto vec = test::generate_string_vec(test_size);
                test::insert(str_tree, vec, [](std::string const& str) {
                    return str;
                });

                test::contains(str_tree, vec, [](auto const& str) {
                    return str;
                });
            }
        }


        /* ----------------------------- */
        /* Hinted insert test std::string */
        /* ----------------------------- */
        if constexpr(test::test_string_hinted_insert) {
            impl::scoped_bool sb{str_tree.active};

            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("HINTED INSERT (std::string)", test_size, i, iters); 
                
                auto vec = test::generate_string_vec(test_size);
                test::hinted_insert(str_tree, vec, [](auto const& str) {
                    return str;
                });

                test::contains(str_tree, vec, [](auto const& str) {
                    return str;
                });
            }
        }

        /* ----------------------------- */
        /* Insert range test std::string */
        /* ----------------------------- */
        if constexpr(test::test_string_insert_range) {
            impl::scoped_bool sb{str_tree.active};

            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("INSERT RANGE (std::string)", test_size, i, iters); 
                
                auto vec = test::generate_string_vec(test_size);
                test::insert_range(str_tree, vec, [](auto const& str) {
                    return str;
                });

                test::contains(str_tree, vec, [](auto const& str) {
                    return str;
                });
            }
        }

        /* ----------------------------- */
        /* Emplace test std::string */
        /* ----------------------------- */
        if constexpr(test::test_string_emplace) {
            impl::scoped_bool sb{str_tree.active};

            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("EMPLACE (std::string)", test_size, i, iters); 
                
                auto vec = test::generate_string_vec(test_size);
                test::emplace(str_tree, vec, [](auto const& str) {
                    return str;
                });

                test::contains(str_tree, vec, [](auto const& str) {
                    return str;
                });
            }
        }

        /* ------------------------------- */
        /* Hinted emplace test std::string */
        /* ------------------------------- */
        if constexpr(test::test_string_hinted_emplace) {
            impl::scoped_bool sb{str_tree.active};

            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("HINTED EMPLACE (std::string)", test_size, i, iters); 
                
                auto vec = test::generate_string_vec(test_size);
                test::hinted_emplace(str_tree, vec, [](auto const& str) {
                    return str;
                });

                test::contains(str_tree, vec, [](auto const& str) {
                    return str;
                });
            }
        }

        /* ---------------------- */
        /* Erase test std::string */
        /* ---------------------- */
        if constexpr(test::test_string_erase) {
            impl::scoped_bool sb{str_tree.active};

            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("ERASE (std::string)", test_size, i, iters);
                
                auto vec = test::generate_string_vec(test_size);
                str_tree.insert(std::begin(vec), std::end(vec));

                test::erase(str_tree, vec, [](auto const& str) {
                    return str;
                });
            
                test::empty(str_tree);
            }
        }

        /* --------------------------- */
        /* operator== test std::string */
        /* --------------------------- */
        if constexpr(test::test_string_eq) {
            impl::scoped_bool sb{str_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("OPERATOR== (std::string)", test_size, i, iters);

                auto vec = test::generate_string_vec(test_size);
                str_tree.clear();
                str_tree.insert(std::begin(vec), std::end(vec));

                test::eq(str_tree, vec);
            }
        }

        /* --------------------------- */
        /* operator!= test std::string */
        /* --------------------------- */
        if constexpr(test::test_string_neq) {
            impl::scoped_bool sb{str_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("OPERATOR!= (std::string)", test_size, i, iters);

                auto vec = test::generate_string_vec(test_size);
                str_tree.clear();
                str_tree.insert(std::begin(vec), std::end(vec));

                test::neq(str_tree, vec);
            }
        }

        /* -------------------------- */
        /* operator< test std::string */
        /* -------------------------- */
        if constexpr(test::test_string_less) {
            impl::scoped_bool sb{str_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;

            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("OPERATOR< (std::string)", test_size, i, iters);

                auto vec = test::generate_string_vec(test_size);
                test::less(str_tree, vec, [](auto& str) {
                    return str + "a";
                });
            }
        }

        /* -------------------------- */
        /* operator> test std::string */
        /* -------------------------- */
        if constexpr(test::test_string_greater) {
            impl::scoped_bool sb{str_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;

            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("OPERATOR> (std::string)", test_size, i, iters);

                auto vec = test::generate_string_vec(test_size);
                test::greater(str_tree, vec, [](auto& str) {
                    return str + "a";
                });
            }
        }

        /* --------------------------- */
        /* operator<= test std::string */
        /* --------------------------- */
        if constexpr(test::test_string_less_or_eq) {
            impl::scoped_bool sb{str_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;

            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("OPERATOR<= (std::string)", test_size, i, iters);

                auto vec = test::generate_string_vec(test_size);
                test::less_or_eq(str_tree, vec, [](auto& str) {
                    return str + "a";
                });
            }
        }

        /* --------------------------- */
        /* operator>= test std::string */
        /* --------------------------- */
        if constexpr(test::test_string_greater_or_eq) {
            impl::scoped_bool sb{str_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;

            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("OPERATOR>= (std::string)", test_size, i, iters);

                auto vec = test::generate_string_vec(test_size);
                test::greater_or_eq(str_tree, vec, [](auto& str) {
                    return str + "a";
                });
            }
        }

        /* ------------------------- */
        /* Iterator test std::string */
        /* ------------------------- */
        if constexpr(test::test_string_iters) {
            impl::scoped_bool sb{str_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("ITERS (std::string)", test_size, i, iters);
                auto vec = test::generate_string_vec(test_size);
                test::iters(str_tree, vec);
            }
        }

        /* -------------------------------- */
        /* Copy ctor test pair<int, double> */
        /* -------------------------------- */
        if constexpr(test::test_pair_copy_ctor) {
            impl::scoped_bool sb{pair_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                auto vec = test::generate_pair_vec(test_size);
                test::print_heading("COPY CTOR (std::pair<int, double>)", test_size, i, iters);
                pair_tree.insert(std::begin(vec), std::end(vec));
        
                test::copy_ctor(pair_tree, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });
            }
        }

        /* -------------------------------- */
        /* Move ctor test pair<int, double> */
        /* -------------------------------- */
        if constexpr(test::test_pair_move_ctor) {
            impl::scoped_bool sb{pair_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("MOVE CTOR (std::pair<int, double>)", test_size, i, iters);
                auto vec = test::generate_pair_vec(test_size);
                pair_tree.insert(std::begin(vec), std::end(vec));
        
                test::move_ctor(pair_tree, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });
            }
        }

        /* -------------------------------------- */
        /* Copy assignment test pair<int, double> */
        /* -------------------------------------- */
        if constexpr(test::test_pair_copy_assignment) {
            impl::scoped_bool sb{pair_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("COPY ASSIGNMENT (std::pair<int, double>)", test_size, i, iters);
                auto vec = test::generate_pair_vec(test_size);
                pair_tree.insert(std::begin(vec), std::end(vec));
        
                test::copy_assignment(pair_tree, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });
            }
        }

        /* -------------------------------------- */
        /* Move assignment test pair<int, double> */
        /* -------------------------------------- */
        if constexpr(test::test_pair_move_assignment) {
            impl::scoped_bool sb{pair_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("MOVE ASSIGNMENT (std::pair<int, double>)", test_size, i, iters);
                auto vec = test::generate_pair_vec(test_size);
                pair_tree.insert(std::begin(vec), std::end(vec));
        
                test::move_assignment(pair_tree, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });
            }
        }

        /* ---------------------------- */
        /* Empty test pair<int, double> */
        /* ---------------------------- */
        if constexpr(test::test_pair_empty) {
            impl::scoped_bool(pair_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                test::print_heading("EMPTY (std::pair<int, double>)", 1, i, iters);
                test::empty(pair_tree);
            }
        }

        /* --------------------------- */
        /* Size test pair<int, double> */
        /* --------------------------- */
        if constexpr(test::test_pair_size) {
            impl::scoped_bool(pair_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("SIZE (std::pair<int, double>)", 1, i, iters);
                auto vec = test::generate_pair_vec(test_size);
                test::size(pair_tree, vec);
            }
        }

        /* --------------------------------- */
        /* Clear test std::pair<int, double> */
        /* --------------------------------- */
        if constexpr(test::test_pair_clear) {
            impl::scoped_bool(pair_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                test::print_heading("CLEAR (std::pair<int, double>)", 1, i, iters);
                test::clear(pair_tree);
            }
        }

        /* ------------------------------------ */
        /* Contains test std::pair<int, double> */
        /* ------------------------------------ */
        if constexpr(test::test_pair_contains) {
            impl::scoped_bool(pair_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("CONTAINS (std::pair<int, double>)", test_size, i, iters);
                auto vec = test::generate_pair_vec(test_size);
                pair_tree.insert(std::begin(vec), std::end(vec));
                test::contains(pair_tree, vec, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });
            }
        }

        /* --------------------------------- */
        /* Count test std::pair<int, double> */
        /* --------------------------------- */
        if constexpr(test::test_pair_count) {
            impl::scoped_bool(pair_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("COUNT (std::pair<int, double>)", test_size, i, iters);
                auto vec = test::generate_pair_vec(test_size);
                pair_tree.insert(std::begin(vec), std::end(vec));
                test::count(pair_tree, vec, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });
            }
        }

        /* -------------------------------- */
        /* Find test std::pair<int, double> */
        /* -------------------------------- */
        if constexpr(test::test_pair_find) {
            impl::scoped_bool(pair_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("FIND (std::pair<int, double>)", test_size, i, iters);
                auto vec = test::generate_pair_vec(test_size);
                test::find(pair_tree, vec, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });
            }
        }

        /* ------------------------------- */
        /* Swap test std::pair<int,double> */
        /* ------------------------------- */
        if constexpr(test::test_pair_swap) {
            impl::scoped_bool(pair_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                auto vec = test::generate_pair_vec(test_size);
                test::print_heading("SWAP (std::pair<int, double>)", test_size, i, iters);
                pair_tree.clear();
                pair_tree.insert(std::begin(vec), std::end(vec));

                test::trbt_trace_type<rbtree<std::pair<int, double>>> tree;
                test_size = test_size_dis(mt);
                vec = test::generate_pair_vec(test_size);
                tree.insert(std::begin(vec), std::end(vec));

                test::swap(pair_tree, tree, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });
            }
        }

        /* --------------------------------------- */
        /* lower_bound test std::pair<int, double> */
        /* --------------------------------------- */
        if constexpr(test::test_pair_lower_bound) {
            impl::scoped_bool(pair_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                auto vec = test::generate_pair_vec(test_size);
                test::print_heading("LOWER BOUND (std::pair<int, double>)", test_size, i, iters);
                pair_tree.insert(std::begin(vec), std::end(vec));
                vec = test::generate_pair_vec(test_size);
                test::lower_bound(pair_tree, vec);
            }
        }

        /* --------------------------------------- */
        /* upper_bound test std::pair<int, double> */
        /* --------------------------------------- */
        if constexpr(test::test_pair_upper_bound) {
            impl::scoped_bool(pair_tree.active);
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                auto vec = test::generate_pair_vec(test_size);
                test::print_heading("UPPER BOUND (std::pair<int, double>)", test_size, i, iters);
                pair_tree.insert(std::begin(vec), std::end(vec));
                vec = test::generate_pair_vec(test_size);
                test::upper_bound(pair_tree, vec);
            }
        }

        /* ---------------------------------- */
        /* Insert test std::pair<int, double> */
        /* ---------------------------------- */
        if constexpr(test::test_pair_insert) {
            impl::scoped_bool sb{pair_tree.active};

            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("INSERT (std::pair<int, double>)", test_size, i, iters); 
                
                auto vec = test::generate_pair_vec(test_size);
                test::insert(pair_tree, vec, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });

                test::contains(pair_tree, vec, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });
            }
        }

        /* ----------------------------------------- */
        /* Hinted insert test std::pair<int, double> */
        /* ----------------------------------------- */
        if constexpr(test::test_pair_hinted_insert) {
            impl::scoped_bool sb{pair_tree.active};

            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("HINTED INSERT (std::pair<int, double>)", test_size, i, iters); 
                auto vec = test::generate_pair_vec(test_size);
                test::hinted_insert(pair_tree, vec, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });

                test::contains(pair_tree, vec, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });
            }
        }

        /* ---------------------------------------- */
        /* Insert range test std::pair<int, double> */
        /* ---------------------------------------- */
        if constexpr(test::test_pair_insert_range) {
            impl::scoped_bool sb{pair_tree.active};

            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("INSERT RANGE (std::pair<int, double>)", test_size, i, iters); 
                auto vec = test::generate_pair_vec(test_size);
                test::insert_range(pair_tree, vec, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });

                test::contains(pair_tree, vec, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });
            }
        }

        /* ----------------------------------- */
        /* Emplace test std::pair<int, double> */
        /* ----------------------------------- */
        if constexpr(test::test_pair_emplace) {
            impl::scoped_bool sb{pair_tree.active};

            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("EMPLACE (std::pair<int, double>)", test_size, i, iters); 
                auto vec = test::generate_pair_vec(test_size);
                test::emplace(pair_tree, vec, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });

                test::contains(pair_tree, vec, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });
            }
        }

        /* ------------------------------------------ */
        /* Hinted emplace test std::pair<int, double> */
        /* ------------------------------------------ */
        if constexpr(test::test_pair_hinted_emplace) {
            impl::scoped_bool sb{pair_tree.active};

            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("HINTED EMPLACE (std::pair<int, double>)", test_size, i, iters); 
                auto vec = test::generate_pair_vec(test_size);
                test::hinted_emplace(pair_tree, vec, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });

                test::contains(pair_tree, vec, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });
            }
        }

        /* --------------------------------- */
        /* Erase test std::pair<int, double> */
        /* --------------------------------- */
        if constexpr(test::test_pair_erase) {
            impl::scoped_bool sb{pair_tree.active};

            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("ERASE (std::pair<int, double>)", test_size, i, iters);

                auto vec = test::generate_pair_vec(6);
                pair_tree.insert(std::begin(vec), std::end(vec));

                test::erase(pair_tree, vec, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });

                test::empty(pair_tree);
            }
        }

        /* -------------------------------------- */
        /* operator== test std::pair<int, double> */
        /* -------------------------------------- */
        if constexpr(test::test_pair_eq) {
            impl::scoped_bool sb{pair_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("OPERATOR== (std::pair<int, double>)", test_size, i, iters);

                auto vec = test::generate_pair_vec(test_size);

                test::eq(pair_tree, vec);
            }
        }

        /* -------------------------------------- */
        /* operator!= test std::pair<int, double> */
        /* -------------------------------------- */
        if constexpr(test::test_pair_neq) {
            impl::scoped_bool sb{pair_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("OPERATOR!= (std::pair<int, double>)", test_size, i, iters);

                auto vec = test::generate_pair_vec(test_size);

                test::neq(pair_tree, vec);
            }
        }

        /* ------------------------------------- */
        /* operator< test std::pair<int, double> */
        /* ------------------------------------- */
        if constexpr(test::test_pair_less) {
            impl::scoped_bool sb{pair_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;

            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("OPERATOR< (std::pair<int, double>)", test_size, i, iters);

                auto vec = test::generate_pair_vec(test_size);
                test::less(pair_tree, vec, [](auto const& pair) {
                    return impl::remove_cvref_t<decltype(pair)>(pair.first + 1, pair.second) ;
                });
            }
        }

        /* ------------------------------------- */
        /* operator> test std::pair<int, double> */
        /* ------------------------------------- */
        if constexpr(test::test_pair_greater) {
            impl::scoped_bool sb{pair_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;

            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("OPERATOR> (std::pair<int, double>)", test_size, i, iters);

                auto vec = test::generate_pair_vec(test_size);
                test::greater(pair_tree, vec, [](auto const& pair) {
                    return impl::remove_cvref_t<decltype(pair)>(pair.first + 1, pair.second);
                });
            }
        }

        /* -------------------------------------- */
        /* operator<= test std::pair<int, double> */
        /* -------------------------------------- */
        if constexpr(test::test_pair_less_or_eq) {
            impl::scoped_bool sb{pair_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;

            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("OPERATOR<= (std::pair<int, double>)", test_size, i, iters);

                auto vec = test::generate_pair_vec(test_size);
                test::less_or_eq(pair_tree, vec, [](auto const& pair) {
                    return impl::remove_cvref_t<decltype(pair)>(pair.first + 1, pair.second);
                });
            }
        }

        /* -------------------------------------- */
        /* operator>= test std::pair<int, double> */
        /* -------------------------------------- */
        if constexpr(test::test_pair_greater_or_eq) {
            impl::scoped_bool sb{pair_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;

            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("OPERATOR>= (std::pair<int, double>)", test_size, i, iters);

                auto vec = test::generate_pair_vec(test_size);
                test::greater_or_eq(pair_tree, vec, [](auto const& pair) {
                    return impl::remove_cvref_t<decltype(pair)>(pair.first + 1, pair.second);
                });
            }
        }

        /* ------------------------------------ */
        /* Iterator test std::pair<int, double> */
        /* ------------------------------------ */
        if constexpr(test::test_pair_iters) {
            impl::scoped_bool sb{pair_tree.active};
            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("ITERS (std::pair<int, double>)", test_size, i, iters);
                auto vec = test::generate_pair_vec(test_size);
                test::iters(pair_tree, vec);
            }
        }
                    

        /* ---------------------------------------------- */
        /* Emplace test std::pair<int, double>, piecewise */
        /* ---------------------------------------------- */
        if constexpr(test::test_pair_piecewise_emplace) {
            impl::scoped_bool sb{pair_tree.active};

            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("EMPLACE (std::pair<int, double>, piecewise)", test_size, i, iters); 
                auto vec = test::generate_int_vec(test_size);
                std::vector<std::pair<int, double>> pvec(vec.size());
                std::transform(std::begin(vec), std::end(vec), std::begin(pvec), [](int i) {
                    return std::make_pair(i, double{});
                });

                test::pair_emplace(pair_tree, vec, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });

                test::contains(pair_tree, pvec, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });
            }
        }

        /* ----------------------------------------------------- */
        /* Hinted emplace test std::pair<int, double>, piecewise */
        /* ----------------------------------------------------- */
        if constexpr(test::test_pair_piecewise_hinted_emplace) {
            impl::scoped_bool sb{pair_tree.active};

            iters = iter_dis(mt);
            total_iters += iters;
            for(int i = 0; i < iters; i++) {
                auto test_size = test_size_dis(mt);
                test::print_heading("HINTED EMPLACE (std::pair<int, double>, piecewise)", test_size, i, iters); 
                auto vec = test::generate_int_vec(test_size);
                std::vector<std::pair<int, double>> pvec(vec.size());
                std::transform(std::begin(vec), std::end(vec), std::begin(pvec), [](int i) {
                    return std::make_pair(i, double{});
                });

                test::pair_hinted_emplace(pair_tree, vec, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });

                test::contains(pair_tree, pvec, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });
            }
        }

        std::cout << "Finished " << total_iters << " tests successfully\n";
    }
    catch(std::runtime_error& err) {
        if(int_tree.active)
            int_tree.print_trace(test::trbt_trace_stream);
        else if(str_tree.active)
            str_tree.print_trace(test::trbt_trace_stream);
        else
            pair_tree.print_trace(test::trbt_trace_stream);

        test::trbt_trace_stream << err.what() << "\n";
    }

    return 0;
}
