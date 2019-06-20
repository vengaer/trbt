#define TRBT_DEBUG
#include "trbt.h"
#include <iostream>
#include <random>

template <trbt::impl::debug::InsertionMethod method>
void run(int iters) {
    using namespace trbt::impl::debug;

    int constexpr min_size = 0;
    int constexpr max_size = 5000;
    
    std::mt19937 mt{std::random_device{}()};
    std::uniform_int_distribution<> dist{min_size, max_size};

    for(int i = 0; i < iters; i++) {
        std::cout << "Iteration " << i << "\n";
        run_test<method>(dist(mt));
    }
}

int main() {
    using namespace trbt::impl::debug;

    int constexpr per_method_iters = 2000;
    int total = 0;


    run<InsertionMethod::Insert>(per_method_iters);
    total += per_method_iters;

    run<InsertionMethod::HintedInsert>(per_method_iters);
    total += per_method_iters;

    run<InsertionMethod::InsertRange>(per_method_iters);
    total += per_method_iters;

    run<InsertionMethod::Emplace>(per_method_iters);
    total += per_method_iters;

    run<InsertionMethod::HintedEmplace>(per_method_iters);
    total += per_method_iters;

    std::cout << "Finished " << total << " tests successfully\n";

    return 0;
}

