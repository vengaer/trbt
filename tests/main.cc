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
    std::uniform_int_distribution<> test_size_dis(0, 1500);
    
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
            ++total_iters;

            auto test_size = test_size_dis(mt);
            test::print_heading("COPY CTOR (int)", test_size);
            auto vec = test::generate_int_vec(test_size);
            int_tree.insert(std::begin(vec), std::end(vec));
    
            test::copy_ctor(int_tree, [](int i) {
                return std::to_string(i);
            });
            int_tree.clear();
        }

        /* ------------------ */
        /* Move ctor test int */
        /* ------------------ */
        if constexpr(test::test_int_move_ctor) {
            impl::scoped_bool sb{int_tree.active};
            ++total_iters;

            auto test_size = test_size_dis(mt);
            test::print_heading("MOVE CTOR (int)", test_size);
            auto vec = test::generate_int_vec(test_size);
            int_tree.insert(std::begin(vec), std::end(vec));
    
            test::move_ctor(int_tree, [](int i) {
                return std::to_string(i);
            });
            int_tree.clear();
        }

        /* ------------------------ */
        /* Copy assignment test int */
        /* ------------------------ */
        if constexpr(test::test_int_copy_assignment) {
            impl::scoped_bool sb{int_tree.active};
            ++total_iters;

            auto test_size = test_size_dis(mt);
            test::print_heading("COPY ASSIGNMENT (int)", test_size);
            auto vec = test::generate_int_vec(test_size);
            int_tree.insert(std::begin(vec), std::end(vec));
    
            test::copy_assignment(int_tree, [](int i) {
                return std::to_string(i);
            });
            int_tree.clear();
        }

        /* ------------------------ */
        /* Move assignment test int */
        /* ------------------------ */
        if constexpr(test::test_int_move_assignment) {
            impl::scoped_bool sb{int_tree.active};
            ++total_iters;

            auto test_size = test_size_dis(mt);
            test::print_heading("MOVE ASSIGNMENT (int)", test_size);
            auto vec = test::generate_int_vec(test_size);
            int_tree.insert(std::begin(vec), std::end(vec));
    
            test::move_assignment(int_tree, [](int i) {
                return std::to_string(i);
            });
            int_tree.clear();
        }

        /* -------------- */
        /* Empty test int */
        /* -------------- */
        if constexpr(test::test_int_empty) {
            impl::scoped_bool(int_tree.active);
            test::print_heading("EMPTY (int)");
            ++total_iters;
            test::empty(int_tree);
            int_tree.clear();
        }

        /* ------------- */
        /* Size test int */
        /* ------------- */
        if constexpr(test::test_int_size) {
            impl::scoped_bool(int_tree.active);
            test::print_heading("SIZE (int)");
            ++total_iters;
            auto test_size = test_size_dis(mt);
            auto vec = test::generate_int_vec(test_size);
            test::size(int_tree, vec);
            int_tree.clear();
        }

        /* -------------- */
        /* Clear test int */
        /* -------------- */
        if constexpr(test::test_int_clear) {
            impl::scoped_bool(int_tree.active);
            test::print_heading("CLEAR (int)");
            ++total_iters;
            test::clear(int_tree);
            int_tree.clear();
        }

        /* ----------------- */
        /* Contains test int */
        /* ----------------- */
        if constexpr(test::test_int_contains) {
            impl::scoped_bool(int_tree.active);
            test::print_heading("CONTAINS (int)");
            ++total_iters;
            auto test_size = test_size_dis(mt);
            auto vec = test::generate_int_vec(test_size);
            int_tree.insert(std::begin(vec), std::end(vec));
            test::contains(int_tree, vec, [](int i) {
                return std::to_string(i);
            });
            int_tree.clear();
        }

        /* -------------- */
        /* Count test int */
        /* -------------- */
        if constexpr(test::test_int_count) {
            impl::scoped_bool(int_tree.active);
            test::print_heading("COUNT (int)");
            ++total_iters;
            auto test_size = test_size_dis(mt);
            auto vec = test::generate_int_vec(test_size);
            int_tree.insert(std::begin(vec), std::end(vec));
            test::count(int_tree, vec, [](int i) {
                return std::to_string(i);
            });
            int_tree.clear();
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
                
                int_tree.clear();
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

                int_tree.clear();
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

                int_tree.clear();
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

                int_tree.clear();
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

                int_tree.clear();
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

        /* -------------------------- */
        /* Copy ctor test std::string */
        /* -------------------------- */
        if constexpr(test::test_string_copy_ctor) {
            impl::scoped_bool sb{str_tree.active};
            ++total_iters;
            auto test_size = test_size_dis(mt);
            test::print_heading("COPY CTOR (std::string)", test_size);
            auto vec = test::generate_string_vec(test_size);
            str_tree.insert(std::begin(vec), std::end(vec));
    
            test::copy_ctor(str_tree, [](auto const& str) {
                return str;
            });

            str_tree.clear();
        }

        /* -------------------------- */
        /* Move ctor test std::string */
        /* -------------------------- */
        if constexpr(test::test_string_move_ctor) {
            impl::scoped_bool sb{str_tree.active};
            ++total_iters;
            auto test_size = test_size_dis(mt);
            test::print_heading("MOVE CTOR (std::string)", test_size);
            auto vec = test::generate_string_vec(test_size);
            str_tree.insert(std::begin(vec), std::end(vec));
    
            test::move_ctor(str_tree, [](auto const& str) {
                return str;
            });

            str_tree.clear();
        }

        /* -------------------------------- */
        /* Copy assignment test std::string */
        /* -------------------------------- */
        if constexpr(test::test_string_copy_assignment) {
            impl::scoped_bool sb{str_tree.active};
            ++total_iters;
            auto test_size = test_size_dis(mt);
            test::print_heading("COPY ASSIGNMENT (std::string)", test_size);
            auto vec = test::generate_string_vec(test_size);
            str_tree.insert(std::begin(vec), std::end(vec));
    
            test::copy_assignment(str_tree, [](auto const& str) {
                return str;
            });

            str_tree.clear();
        }

        /* -------------------------- */
        /* Move assignment test std::string */
        /* -------------------------- */
        if constexpr(test::test_string_move_assignment) {
            impl::scoped_bool sb{str_tree.active};
            ++total_iters;
            auto test_size = test_size_dis(mt);
            test::print_heading("MOVE ASSIGNMENT (std::string)", test_size);
            auto vec = test::generate_string_vec(test_size);
            str_tree.insert(std::begin(vec), std::end(vec));
    
            test::move_assignment(str_tree, [](auto const& str) {
                return str;
            });

            str_tree.clear();
        }

        /* ---------------------- */
        /* Empty test std::string */
        /* ---------------------- */
        if constexpr(test::test_string_empty) {
            impl::scoped_bool(str_tree.active);
            test::print_heading("EMPTY (std::string)");
            ++total_iters;
            test::empty(str_tree);
            str_tree.clear();
        }

        /* --------------------- */
        /* Size test std::string */
        /* --------------------- */
        if constexpr(test::test_string_size) {
            impl::scoped_bool(str_tree.active);
            ++total_iters;
            test::print_heading("SIZE (std::string)");
            auto test_size = test_size_dis(mt);
            auto vec = test::generate_string_vec(test_size);
            test::size(str_tree, vec);
            str_tree.clear();
        }

        /* ---------------------- */
        /* Clear test std::string */
        /* ---------------------- */
        if constexpr(test::test_string_clear) {
            impl::scoped_bool(str_tree.active);
            test::print_heading("CLEAR (std::string)");
            ++total_iters;
            test::clear(str_tree);
            str_tree.clear();
        }

        /* ------------------------- */
        /* Contains test std::string */
        /* ------------------------- */
        if constexpr(test::test_string_contains) {
            impl::scoped_bool(str_tree.active);
            test::print_heading("CONTAINS (std::string)");
            ++total_iters;
            auto test_size = test_size_dis(mt);
            auto vec = test::generate_string_vec(test_size);
            str_tree.insert(std::begin(vec), std::end(vec));
            test::contains(str_tree, vec, [](auto const& str) {
                return str;
            });
            str_tree.clear();
        }

        /* ---------------------- */
        /* Count test std::string */
        /* ---------------------- */
        if constexpr(test::test_string_count) {
            impl::scoped_bool(str_tree.active);
            test::print_heading("COUNT (std::string)");
            ++total_iters;
            auto test_size = test_size_dis(mt);
            auto vec = test::generate_string_vec(test_size);
            str_tree.insert(std::begin(vec), std::end(vec));
            test::count(str_tree, vec, [](auto const& str) {
                return str;
            });
            str_tree.clear();
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

                str_tree.clear();
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

                str_tree.clear();
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
        
                str_tree.clear();
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
            
                str_tree.clear();
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
            
                str_tree.clear();
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

        /* -------------------------------- */
        /* Copy ctor test pair<int, double> */
        /* -------------------------------- */
        if constexpr(test::test_pair_copy_ctor) {
            impl::scoped_bool sb{pair_tree.active};
            ++total_iters;
            auto test_size = test_size_dis(mt);
            test::print_heading("COPY CTOR (std::pair<int, double>)", test_size);
            auto vec = test::generate_pair_vec(test_size);
            pair_tree.insert(std::begin(vec), std::end(vec));
    
            test::copy_ctor(pair_tree, [](auto const& pair) {
                std::ostringstream ss;
                ss << "{" << pair.first << ", " << pair.second << "}";
                return ss.str();
            });

            pair_tree.clear();
        }

        /* -------------------------------- */
        /* Move ctor test pair<int, double> */
        /* -------------------------------- */
        if constexpr(test::test_pair_move_ctor) {
            impl::scoped_bool sb{pair_tree.active};
            ++total_iters;
            auto test_size = test_size_dis(mt);
            test::print_heading("MOVE CTOR (std::pair<int, double>)", test_size);
            auto vec = test::generate_pair_vec(test_size);
            pair_tree.insert(std::begin(vec), std::end(vec));
    
            test::move_ctor(pair_tree, [](auto const& pair) {
                std::ostringstream ss;
                ss << "{" << pair.first << ", " << pair.second << "}";
                return ss.str();
            });

            pair_tree.clear();
        }

        /* -------------------------------------- */
        /* Copy assignment test pair<int, double> */
        /* -------------------------------------- */
        if constexpr(test::test_pair_copy_assignment) {
            impl::scoped_bool sb{pair_tree.active};
            ++total_iters;
            auto test_size = test_size_dis(mt);
            test::print_heading("COPY ASSIGNMENT (std::pair<int, double>)", test_size);
            auto vec = test::generate_pair_vec(test_size);
            pair_tree.insert(std::begin(vec), std::end(vec));
    
            test::copy_assignment(pair_tree, [](auto const& pair) {
                std::ostringstream ss;
                ss << "{" << pair.first << ", " << pair.second << "}";
                return ss.str();
            });

            pair_tree.clear();
        }

        /* -------------------------------------- */
        /* Move assignment test pair<int, double> */
        /* -------------------------------------- */
        if constexpr(test::test_pair_move_assignment) {
            impl::scoped_bool sb{pair_tree.active};
            ++total_iters;
            auto test_size = test_size_dis(mt);
            test::print_heading("MOVE ASSIGNMENT (std::pair<int, double>)", test_size);
            auto vec = test::generate_pair_vec(test_size);
            pair_tree.insert(std::begin(vec), std::end(vec));
    
            test::move_assignment(pair_tree, [](auto const& pair) {
                std::ostringstream ss;
                ss << "{" << pair.first << ", " << pair.second << "}";
                return ss.str();
            });

            pair_tree.clear();
        }

        /* ---------------------------- */
        /* Empty test pair<int, double> */
        /* ---------------------------- */
        if constexpr(test::test_pair_empty) {
            impl::scoped_bool(pair_tree.active);
            test::print_heading("EMPTY (std::pair<int, double>)");
            ++total_iters;
            test::empty(pair_tree);
            pair_tree.clear();
        }

        /* --------------------------- */
        /* Size test pair<int, double> */
        /* --------------------------- */
        if constexpr(test::test_pair_size) {
            impl::scoped_bool(pair_tree.active);
            test::print_heading("SIZE (std::pair<int, double>)");
            ++total_iters;
            auto test_size = test_size_dis(mt);
            auto vec = test::generate_pair_vec(test_size);
            test::size(pair_tree, vec);
            pair_tree.clear();
        }

        /* --------------------------------- */
        /* Clear test std::pair<int, double> */
        /* --------------------------------- */
        if constexpr(test::test_pair_clear) {
            impl::scoped_bool(pair_tree.active);
            test::print_heading("CLEAR (std::pair<int, double>)");
            ++total_iters;
            test::clear(pair_tree);
            pair_tree.clear();
        }

        /* ------------------------------------ */
        /* Contains test std::pair<int, double> */
        /* ------------------------------------ */
        if constexpr(test::test_pair_contains) {
            impl::scoped_bool(pair_tree.active);
            test::print_heading("CONTAINS (std::pair<int, double>)");
            ++total_iters;
            auto test_size = test_size_dis(mt);
            auto vec = test::generate_pair_vec(test_size);
            pair_tree.insert(std::begin(vec), std::end(vec));
            test::contains(pair_tree, vec, [](auto const& pair) {
                std::ostringstream ss;
                ss << "{" << pair.first << ", " << pair.second << "}";
                return ss.str();
            });
            pair_tree.clear();
        }

        /* --------------------------------- */
        /* Count test std::pair<int, double> */
        /* --------------------------------- */
        if constexpr(test::test_pair_count) {
            impl::scoped_bool(pair_tree.active);
            test::print_heading("COUNT (std::pair<int, double>)");
            ++total_iters;
            auto test_size = test_size_dis(mt);
            auto vec = test::generate_pair_vec(test_size);
            pair_tree.insert(std::begin(vec), std::end(vec));
            test::count(pair_tree, vec, [](auto const& pair) {
                std::ostringstream ss;
                ss << "{" << pair.first << ", " << pair.second << "}";
                return ss.str();
            });
            pair_tree.clear();
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
            
                pair_tree.clear();
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

                pair_tree.clear();
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

                pair_tree.clear();
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

                pair_tree.clear();
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
            
                pair_tree.clear();
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

                auto vec = test::generate_pair_vec(test_size);
        
                test::insert(pair_tree, vec, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });

                test::erase(pair_tree, vec, [](auto const& pair) {
                    std::ostringstream ss;
                    ss << "{" << pair.first << ", " << pair.second << "}";
                    return ss.str();
                });

                test::empty(pair_tree);
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

                pair_tree.clear();
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

                pair_tree.clear();
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
