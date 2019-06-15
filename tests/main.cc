#define TRBT_DEBUG
#include "trbt.h"
#include <iostream>
#include <set>

struct test {
    test() : i{} {}
    test(int j) : i{j} { }
    test(test const&) = delete;
    test(test&&) = default;
    test& operator=(test const&) = delete;
    test& operator=(test&&) = default;

    friend bool operator<(test const& l, test const& r) {
        return l.i < r.i;
    }

    friend bool operator>(test const& l, test const& r) {
        return l.i > r.i;
    } 
        
    private:
        int i;
};

int main() {
    trbt::rbtree t{1,2,3,4,5,6,7,9,8};
    trbt::rbtree r{std::pair{1,2}, std::pair{2, 4.0}};
    for(int i = 0; i < 10; i++)
        r.emplace(i,i);

    r.print();
    t.print();
    t.assert_properties();
    trbt::rbtree<test> tree{};
    //trbt::impl::debug::run_test<trbt::impl::debug::InsertionMethod::HintedEmplace>();
    //typename decltype(t)::value_compare a;
    // TODO: Test leftmost and rightmost
    for(auto i : t)
        std::cout << i << " ";
    std::cout << "\n";
    std::cout << std::boolalpha << std::is_same_v<int, trbt::impl::nth_type_t<2, int, std::string, int>> << "\n";
}

