#define TRBT_DEBUG
#include "trbt.h"
#include <iostream>
#include <thread>
int main() {
    trbt::red_black_tree t{5,4,6,7,8};
    for(auto const& v : t)
        std::cout << v << "\n";
    trbt::red_black_tree t2{std::make_pair(1,1)};
    t2.print();
    trbt::red_black_tree t3{t2};
    t3.emplace(4, 4);
    t3.print();
    std::cout << std::boolalpha << (t2 < t3) << "\n";
    t2 = t3;
    t2.print();
    trbt::red_black_tree<std::pair<int const, double>> t6{std::make_pair(1, 1.0)};
    t6.print();
    //trbt::impl::debug::run_test();
}
