#define TRBT_DEBUG
#include "trbt.h"
#include <iostream>
#include <set>
int main() {
    trbt::rbtree t{1,2,3,4,5,6,7,9,8};
    trbt::rbtree r{std::pair{1,2}, std::pair{2, 4.0}};
    for(int i = 0; i < 10; i++)
        r.emplace(i,i);

    r.print();
    t.print();
    t.assert_properties();
    trbt::impl::debug::run_test<trbt::impl::debug::InsertionMethod::HintedEmplace>();
    //typename decltype(t)::value_compare a;
    // TODO: Test leftmost and rightmost
    for(auto i : t)
        std::cout << i << " ";
    std::cout << "\n";
}

