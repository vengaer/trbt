#define TRBT_DEBUG
#include "trbt.h"
#include <iostream>
#include <set>
int main() {
    trbt::red_black_tree t{1,2,3,4,5,6,7,9,8};
    trbt::red_black_tree r{std::pair{1,2}, std::pair{2, 4.0}};
    r.print();
    trbt::impl::debug::run_test();
    for(auto i : t)
        std::cout << i << "\n";
    std::set<int> s;
    s.insert(1);
    s.insert(2);
    s.insert(3);
}
