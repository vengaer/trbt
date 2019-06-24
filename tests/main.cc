#define TRBT_DEBUG
#include "trbt.h"
#include <iostream>
#include <random>

using namespace trbt::impl;
using namespace trbt::impl::debug;

enum class InsertionMethod {
    Insert,
    InsertRange,
    HintedInsert,
    Emplace,
    HintedEmplace
};

template <typename Tree>
void test_dmost(Tree const& tree, Direction dir);

template <InsertionMethod method>
void run_randomized(int iters);

template <InsertionMethod method>
std::string insertion_method();

template <InsertionMethod method>
void run_test(int test_size);

template <InsertionMethod method>
void run_randomized(int iters);

int main() {
    int constexpr per_method_iters = 1000;
    int total = 0;

    run_randomized<InsertionMethod::Insert>(per_method_iters);
    total += per_method_iters;

    run_randomized<InsertionMethod::HintedInsert>(per_method_iters);
    total += per_method_iters;

    run_randomized<InsertionMethod::InsertRange>(per_method_iters);
    total += per_method_iters;

    run_randomized<InsertionMethod::Emplace>(per_method_iters);
    total += per_method_iters;

    run_randomized<InsertionMethod::HintedEmplace>(per_method_iters);
    total += per_method_iters;

    std::cout << "Finished " << total << " tests successfully\n";

    return 0;
}

template <typename Tree>
void test_dmost(Tree const& tree, Direction dir) {
    int flags;
    std::string msg{"Assertion of "};
    std::string direction;

    if(dir == Direction::Left) {
        flags = tree.assert_leftmost_ok();
        direction = "left";
    }
    else {
        flags = tree.assert_rightmost_ok();
        direction = "right";
    }

    msg += direction + "most encountered the following issues:\n";

    if(thread_error(flags)) 
        msg += "\t" + direction + " thread not set\n";
    if(link_error(flags))
        msg += "\t" + direction + " does not point to sentinel_\n";
    if(node_error(flags))
        msg += "\t" + direction + "most_ is not the " + direction + "most node in tree\n";

    if(flags)
        throw thread_link_exception{std::move(msg)};
}

template <InsertionMethod method>
std::string insertion_method() {
    if constexpr(method == InsertionMethod::InsertRange)
        return "insert range";
    else if constexpr(method == InsertionMethod::HintedInsert)
        return "hinted insert";
    else if constexpr(method == InsertionMethod::Insert)
        return "insert";
    else if constexpr(method == InsertionMethod::Emplace)
        return "emplace";

    return "hinted emplace";
}

template <InsertionMethod method>
void run_test(int test_size) {
    using namespace trbt;
    std::cout << "Running [" << insertion_method<method>() 
              <<  "] test with size " << test_size << "\n";
    std::mt19937 mt{std::random_device{}()};
    std::vector<int> vals(test_size);

    {
        std::uniform_int_distribution<> dis{-10 * test_size, 10 * test_size};

        for(auto i = 0; i < test_size; i++)
            vals.push_back(dis(mt));
    }

    vals.erase(std::unique(std::begin(vals), std::end(vals)), std::end(vals));

    rbtree rbt{1,2,3};
    
    if constexpr(method == InsertionMethod::InsertRange) {
        rbt.insert(std::begin(vals), std::end(vals));
        rbt.assert_properties_ok();
        test_dmost(rbt, Direction::Left);
        test_dmost(rbt, Direction::Right);
    }
    else {
        for(auto const& v : vals) {
            if constexpr(method == InsertionMethod::Insert)
                rbt.insert(v);
            else if constexpr(method == InsertionMethod::Emplace) {
                rbt.emplace(v);
            }
            else {
                auto it = rbt.upper_bound(v);   
                if constexpr(method == InsertionMethod::HintedInsert)
                    rbt.insert(it, v);
                else
                    rbt.emplace_hint(it, v);
            }
            rbt.assert_properties_ok();
            test_dmost(rbt, Direction::Left);
            test_dmost(rbt, Direction::Right);
        }
    }
    rbt.assert_properties_ok();
    test_dmost(rbt, Direction::Left);
    test_dmost(rbt, Direction::Right);

    for(auto const& v : vals) {
        if(!rbt.contains(v))
            throw value_retention_exception{std::to_string(v) + " not in tree\n"};
    }

    while(vals.size()) {
        std::uniform_int_distribution<> dis{0, static_cast<int>(vals.size()) - 1};
        auto rand = dis(mt);
        auto val = vals[rand];
        rbt.erase(val);
        rbt.assert_properties_ok();
        test_dmost(rbt, Direction::Left);
        test_dmost(rbt, Direction::Right);

        vals.erase(std::remove_if(std::begin(vals), std::end(vals), [val](int v) { 
            return val == v; 
        }), std::end(vals));

        for(auto const& v : vals) {
            if(!rbt.contains(v))
                throw value_retention_exception{std::to_string(v) + " is not in tree\n"};
        }
    }
    
    std::cout << "Test completed\n\n";
}

template <InsertionMethod method>
void run_randomized(int iters) {
    using namespace trbt::impl::debug;

    int constexpr min_size = 0;
    int constexpr max_size = 1500;
    
    std::mt19937 mt{std::random_device{}()};
    std::uniform_int_distribution<> dist{min_size, max_size};

    for(int i = 0; i < iters; i++) {
        std::cout << "Iteration " << i << "\n";
        run_test<method>(dist(mt));
    }
}
