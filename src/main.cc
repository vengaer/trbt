#define TRBT_DEBUG
#include "trbt.h"
#include <iostream>
int main() {
    trbt::red_black_tree t{1,2,3,4,5,6,7,9,8};
    trbt::red_black_tree r{std::pair{1,2}, std::pair{2, 4.0}};
    r.print();
    for(auto& i : t)
        std::cout << i << "\n";
    trbt::impl::debug::run_test();
}
