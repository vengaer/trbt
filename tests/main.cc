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
            
                test::insert(int_tree, vec, [](int i) {
                    return std::to_string(i);
                });

                test::erase(int_tree, vec, [](int i) {
                    return std::to_string(i);
                });

                test::empty(int_tree);
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
                test::insert(str_tree, vec, [](auto const& str) {
                    return str;
                });

                test::erase(str_tree, vec, [](auto const& str) {
                    return str;
                });
            
                test::empty(str_tree);
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
                std::vector<std::pair<int, double>> pvec(test_size);
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
                std::vector<std::pair<int, double>> pvec(test_size);
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
            int_tree.print_trace(TRACE_STREAM);
        else if(str_tree.active)
            str_tree.print_trace(TRACE_STREAM);
        else
            pair_tree.print_trace(TRACE_STREAM);
    }

    return 0;
}
