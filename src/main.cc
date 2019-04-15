#define TRBT_DEBUG
#include "trbt.h"
#include <iostream>
int main() {
    trbt::red_black_tree t{1,2,3,4,5,6,7,9,8};
    for(auto& i : t)
        std::cout << i << "\n";
    trbt::impl::debug::run_test();
}
