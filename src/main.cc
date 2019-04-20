#define TRBT_DEBUG
#include "trbt.h"
#include <iostream>
#include <set>
int main() {
    trbt::red_black_tree t{1,2,3,4,5,6,7,9,8};
    trbt::red_black_tree r{std::pair{1,2}, std::pair{2, 4.0}};
    r.print();
    t.emplace_hint(std::end(t), 10);
    t.print();
    t.assert_properties();
    //trbt::impl::debug::run_test();
}
