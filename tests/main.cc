#include "trbt_test_framework.h"
#include <random>
#include <sstream>
#include <utility>

int main() {
    using namespace trbt;
    std::mt19937 mt{std::random_device{}()};
    std::uniform_int_distribution<> iter_dis(0, 1000);
    std::uniform_int_distribution<> test_size_dis(0, 1500);
    
    rbtree<int> int_tree;
    rbtree<std::string> str_tree;
    rbtree<std::pair<int, double>> pair_tree;
    
    /* --------------- */
    /* Insert test int */
    /* --------------- */
    auto iters = iter_dis(mt);
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

        test::erase(int_tree, vec, [](int i) {
            return std::to_string(i);
        });
    }

    /* ---------------------- */
    /* Hinted insert test int */
    /* ---------------------- */
    iters = iter_dis(mt);
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

        test::erase(int_tree, vec, [](int i) {
            return std::to_string(i);
        });
    }

    /* --------------------- */
    /* Insert range test int */
    /* --------------------- */
    iters = iter_dis(mt);
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

        test::erase(int_tree, vec, [](int i) {
            return std::to_string(i);
        });
    }

    /* ---------------- */
    /* Emplace test int */
    /* ---------------- */
    iters = iter_dis(mt);
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

        test::erase(int_tree, vec, [](int i) {
            return std::to_string(i);
        });
    }

    /* ----------------------- */
    /* Hinted emplace test int */
    /* ----------------------- */
    iters = iter_dis(mt);
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

        test::erase(int_tree, vec, [](int i) {
            return std::to_string(i);
        });
    }

    /* ----------------------- */
    /* Insert test std::string */
    /* ----------------------- */
    iters = iter_dis(mt);
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

        test::erase(str_tree, vec, [](auto const& str) {
            return str;
        });
    }


    /* ----------------------------- */
    /* Hined insert test std::string */
    /* ----------------------------- */
    iters = iter_dis(mt);
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

        test::erase(str_tree, vec, [](auto const& str) {
            return str;
        });
    }

    /* ----------------------------- */
    /* Insert range test std::string */
    /* ----------------------------- */
    iters = iter_dis(mt);
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

        test::erase(str_tree, vec, [](auto const& str) {
            return str;
        });
    }

    /* ----------------------------- */
    /* Emplace test std::string */
    /* ----------------------------- */
    iters = iter_dis(mt);
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

        test::erase(str_tree, vec, [](auto const& str) {
            return str;
        });
    }

    /* ------------------------------- */
    /* Hinted emplace test std::string */
    /* ------------------------------- */
    iters = iter_dis(mt);
    for(int i = 0; i < iters; i++) {
        auto test_size = test_size_dis(mt);
        test::print_heading("EMPLACE (std::string)", test_size, i, iters); 
        
        auto vec = test::generate_string_vec(test_size);
        test::hinted_emplace(str_tree, vec, [](auto const& str) {
            return str;
        });

        test::contains(str_tree, vec, [](auto const& str) {
            return str;
        });

        test::erase(str_tree, vec, [](auto const& str) {
            return str;
        });
    }
    /* ---------------------------------- */
    /* Insert test std::pair<int, double> */
    /* ---------------------------------- */
    iters = iter_dis(mt);
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

        test::erase(pair_tree, vec, [](auto const& pair) {
            std::ostringstream ss;
            ss << "{" << pair.first << ", " << pair.second << "}";
            return ss.str();
        });
    }


    /* ----------------------------------------- */
    /* Hinted insert test std::pair<int, double> */
    /* ----------------------------------------- */
    iters = iter_dis(mt);
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

        test::erase(pair_tree, vec, [](auto const& pair) {
            std::ostringstream ss;
            ss << "{" << pair.first << ", " << pair.second << "}";
            return ss.str();
        });
    }

    /* ---------------------------------------- */
    /* Insert range test std::pair<int, double> */
    /* ---------------------------------------- */
    iters = iter_dis(mt);
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

        test::erase(pair_tree, vec, [](auto const& pair) {
            std::ostringstream ss;
            ss << "{" << pair.first << ", " << pair.second << "}";
            return ss.str();
        });
    }

    /* ----------------------------------- */
    /* Emplace test std::pair<int, double> */
    /* ----------------------------------- */
    iters = iter_dis(mt);
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

        test::erase(pair_tree, vec, [](auto const& pair) {
            std::ostringstream ss;
            ss << "{" << pair.first << ", " << pair.second << "}";
            return ss.str();
        });
    }

    /* ------------------------------------------ */
    /* Hinted emplace test std::pair<int, double> */
    /* ------------------------------------------ */
    iters = iter_dis(mt);
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

        test::erase(pair_tree, vec, [](auto const& pair) {
            std::ostringstream ss;
            ss << "{" << pair.first << ", " << pair.second << "}";
            return ss.str();
        });
    }

    /* ---------------------------------------------- */
    /* Emplace test std::pair<int, double>, piecewise */
    /* ---------------------------------------------- */
    iters = iter_dis(mt);
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

        test::erase(pair_tree, pvec, [](auto const& pair) {
            std::ostringstream ss;
            ss << "{" << pair.first << ", " << pair.second << "}";
            return ss.str();
        });
    }

    /* ----------------------------------------------------- */
    /* Hinted emplace test std::pair<int, double>, piecewise */
    /* ----------------------------------------------------- */
    iters = iter_dis(mt);
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

        test::erase(pair_tree, pvec, [](auto const& pair) {
            std::ostringstream ss;
            ss << "{" << pair.first << ", " << pair.second << "}";
            return ss.str();
        });
    }

    return 0;
}
