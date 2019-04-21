#define TRBT_DEBUG
#include "trbt.h"
#include <iostream>
#include <set>
int main() {
    trbt::red_black_tree t{1,2,3,4,5,6,7,9,8};
    trbt::red_black_tree r{std::pair{1,2}, std::pair{2, 4.0}};
    for(int i = 0; i < 10; i++)
        r.emplace(i,i);

    r.print();
    t.emplace_hint(std::end(t), 5);
    t.print();
    t.assert_properties();
    trbt::impl::debug::run_test();
    //typename decltype(t)::value_compare a;
}

