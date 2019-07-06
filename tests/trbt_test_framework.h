#ifndef TRBT_TEST_FRAMEWORK_H
#define TRBT_TEST_FRAMEWORK_H

#define TRBT_DEBUG
#pragma once
#include "trbt.h"
#include "trbt_trace_type.h"
#include <array>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>


namespace trbt {

namespace impl {
    template <typename, typename = void>
    struct is_assignable : std::false_type { };

    template <typename T>
    struct is_assignable<T, std::void_t<decltype(std::declval<T&>() = std::declval<T>())>>
        : std::true_type { };

    template <typename T>
    inline bool constexpr is_assignable_v = is_assignable<T>::value;

    class scoped_bool {
        public:
            scoped_bool(bool& var, bool target_state = true)
                : state_{var}, target_state_{target_state} { 
                state_ = target_state_;
            }

            ~scoped_bool() {
                state_ = !target_state_;
            }

            bool get() const {
                return state_;
            }

        private:
            bool& state_;
            bool target_state_;
    };

    struct iterator_exception : std::runtime_error {
        using std::runtime_error::runtime_error;
    };
    
    struct ordering_exception : std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}

namespace test {
    template <typename Tree, typename StringConverter>
    void copy_ctor(Tree& tree, StringConverter sc);

    template <typename Tree, typename StringConverter>
    void move_ctor(Tree& tree, StringConverter sc);

    template <typename Tree, typename StringConverter>
    void copy_assignment(Tree& tree, StringConverter sc);

    template <typename Tree, typename StringConverter>
    void move_assignment(Tree& tree, StringConverter sc);

    template <typename Tree>
    void empty(Tree& tree);

    template <typename Tree, typename T>
    void size(Tree& tree, std::vector<T> const& vals);

    template <typename Tree>
    void clear(Tree& tree);

    template <typename Tree, typename Vec, typename StringConverter>
    void contains(Tree const& tree, Vec const& vals, StringConverter sc);

    template <typename Tree, typename Vec, typename StringConverter>
    void count(Tree const& tree, Vec const& vals, StringConverter sc);

    template <typename Tree, typename Vec, typename StringConverter>
    void find(Tree& tree, Vec& vals, StringConverter sc);

    template <typename Tree, typename StringConverter>
    void swap(Tree& t1, Tree& t2, StringConverter sc);

    template <typename Tree, typename Vec>
    void lower_bound(Tree& tree, Vec& vals);

    template <typename Tree, typename Vec>
    void upper_bound(Tree& tree, Vec& vals);

    template <typename Tree>
    void leftmost(Tree const& tree);

    template <typename Tree>
    void rightmost(Tree const& tree);

    template <typename Tree, typename T, typename StringConverter>
    void insert(Tree& tree, std::vector<T> const& vals, StringConverter sc);

    template <typename Tree, typename T, typename StringConverter>
    void insert_range(Tree& tree, std::vector<T> const& vals, StringConverter sc);

    template <typename Tree, typename T, typename StringConverter>
    void hinted_insert(Tree& tree, std::vector<T> const& vals, StringConverter sc);
    
    template <typename Tree, typename T, typename StringConverter>
    void emplace(Tree& tree, std::vector<T> const& vals, StringConverter sc);

    template <typename Tree, typename T, typename StringConverter>
    void hinted_emplace(Tree& tree, std::vector<T> const& vals, StringConverter sc);

    template <typename Tree, typename T, typename StringConverter>
    void pair_emplace(Tree& tree, std::vector<T> const& vals, StringConverter sc);

    template <typename Tree, typename T, typename StringConverter>
    void pair_hinted_emplace(Tree& tree, std::vector<T> const& vals, StringConverter sc);

    template <typename Tree, typename Vec, typename StringConverter>
    void erase(Tree& tree, Vec& vals, StringConverter sc);

    template <typename Tree, typename Vec>
    void eq(Tree& tree, Vec& vals);

    template <typename Tree, typename Vec>
    void neq(Tree& tree, Vec& vals);

    template <typename Tree, typename Vec, typename Incrementor>
    void less(Tree& tree, Vec const& vals, Incrementor inc);

    template <typename Tree, typename Vec, typename Incrementor>
    void greater(Tree& tree, Vec const& vals, Incrementor inc);

    template <typename Tree, typename Vec, typename Incrementor>
    void less_or_eq(Tree& tree, Vec const& vals, Incrementor inc);

    template <typename Tree, typename Vec, typename Incrementor>
    void greater_or_eq(Tree& tree, Vec const& vals, Incrementor inc);

    template <typename Tree, typename Vec>
    void iters(Tree& tree, Vec& vals);

    std::vector<int> generate_int_vec(std::size_t size);
    std::vector<std::string> generate_string_vec(std::size_t size);
    std::vector<std::pair<int, double>> generate_pair_vec(std::size_t size);

    void print_heading(std::string const& method, std::size_t test_size = 1u, 
            std::size_t current_iter = 0u, std::size_t total_iters = 1u);

    /* Definitions */

    template <typename Tree, typename StringConverter>
    void copy_ctor(Tree& tree, StringConverter sc) {
        using namespace trbt::impl;

        Tree cpy{tree};

        leftmost(cpy);
        rightmost(cpy);

        std::vector vec(std::begin(tree), std::end(tree));

        contains(cpy, vec, sc);
        
        if(cpy.size() != tree.size())
            throw value_retention_exception{"Original and copy are not the same size\n"};
    }

    template <typename Tree, typename StringConverter>
    void move_ctor(Tree& tree, StringConverter sc) {
        using namespace trbt::impl;

        std::vector vec(std::begin(tree), std::end(tree));
        Tree cpy{std::move(tree)};

        leftmost(cpy);
        rightmost(cpy);
        
        leftmost(tree);
        rightmost(tree);

        contains(cpy, vec, sc);

        if(cpy.size() != vec.size())
            throw value_retention_exception{"Original and copy are not the same size\n"};

        if(tree.size() != 0u)
            throw value_retention_exception{"Original is not empty after move"};
    }

    template <typename Tree, typename StringConverter>
    void copy_assignment(Tree& tree, StringConverter sc) {
        using namespace trbt::impl;

        Tree cpy;
        cpy = tree;

        leftmost(cpy);
        rightmost(cpy);

        std::vector vec(std::begin(tree), std::end(tree));

        contains(cpy, vec, sc);
        
        if(cpy.size() != tree.size())
            throw value_retention_exception{"Original and copy are not the same size\n"};
    }

    template <typename Tree, typename StringConverter>
    void move_assignment(Tree& tree, StringConverter sc) {
        using namespace trbt::impl;

        std::vector vec(std::begin(tree), std::end(tree));
        Tree cpy;
        cpy = std::move(tree);

        leftmost(cpy);
        rightmost(cpy);
        
        leftmost(tree);
        rightmost(tree);

        contains(cpy, vec, sc);

        if(cpy.size() != vec.size())
            throw value_retention_exception{"Original and copy are not the same size\n"};

        if(tree.size() != 0u)
            throw value_retention_exception{"Original is not empty after move"};
    }
        
    template <typename Tree>
    void empty(Tree& tree) {
        using namespace trbt::impl;
        tree.clear();

        if(!tree.empty())
            throw value_retention_exception{"Tree not empty after clearing"};

        tree.insert(typename Tree::value_type{});
        if(tree.empty())
            throw value_retention_exception{"Tree claims empty after insertion"};

        tree.clear();
    }

    template <typename Tree, typename T>
    void size(Tree& tree, std::vector<T>& vals) {
        using namespace trbt::impl;

        tree.clear();
        auto i = 0u;

        for(auto const& v : vals) {
            if(tree.size() != i) {
                throw value_retention_exception{"Size " + std::to_string(tree.size()) + 
                        " should be " + std::to_string(i) + "\n"};
            }

            tree.insert(v); 
            ++i;
        }
        if(tree.size() != i) {
                throw value_retention_exception{"Size " + std::to_string(tree.size()) + 
                        " should be " + std::to_string(i) + "\n"};
        }
    }

    template <typename Tree>
    void clear(Tree& tree) {
        using namespace trbt::impl;
        for(int i = 0; i < 2; i++) {
            tree.clear();
        
            if(tree.size() != 0)
                throw value_retention_exception{"Tree size is not 0 after calling clear"};

            if(!tree.empty())
                throw value_retention_exception{"Empty returned false"};
        }
    }

    template <typename Tree, typename Vec, typename StringConverter>
    void contains(Tree const& tree, Vec const& vals, StringConverter sc) {
        using namespace trbt::impl;
        Tree cpy{tree};
        for(auto const& v : vals) {
            if(!tree.contains(v))
                throw value_retention_exception{sc(v) + " not in tree\n"};
            cpy.erase(v);
            if(cpy.contains(v))
                throw value_retention_exception{sc(v) + " in tree after erasure\n"};
        }
    }

    template <typename Tree, typename Vec, typename StringConverter>
    void count(Tree const& tree, Vec const& vals, StringConverter sc) {
        using namespace trbt::impl;
        Tree cpy{tree};
        for(auto const& v : vals) {
            if(!tree.count(v))
                throw value_retention_exception{sc(v) + " not in tree\n"};
            cpy.erase(v);
            if(cpy.count(v))
                throw value_retention_exception{sc(v) + " in tree after erasure\n"};
        }

    }

    template <typename Tree, typename Vec, typename StringConverter>
    void find(Tree& tree, Vec& vals, StringConverter sc) {
        using namespace trbt::impl;
        std::mt19937 mt{std::random_device{}()};
        tree.clear();
        tree.insert(std::begin(vals), std::end(vals));

        for(auto const& v : vals) {
            if(tree.find(v) == std::end(tree))
                throw value_retention_exception{sc(v) + " not in tree\n"};
        }

        std::shuffle(std::begin(vals), std::end(vals), mt);

        for(auto i = 0u; i < vals.size() / 2; i++) {
            tree.erase(vals[i]);

            if(tree.find(vals[i]) != std::end(tree)) {
                throw value_retention_exception{"Searching for " + sc(vals[i]) + 
                        " did not return end even though having been erased\n"};
            }
        }
    }

    template <typename Tree, typename StringConverter>
    void swap(Tree& t1, Tree& t2, StringConverter sc) {
        using namespace trbt::impl;
    
        Tree t1_cpy{t1};
        Tree t2_cpy{t2};

        swap(t1_cpy, t2_cpy);

        if(t1_cpy != t2)
            throw value_retention_exception{"t1 copy not equal to t2"};
        if(t2_cpy != t1)
            throw value_retention_exception{"t2 copy not equal to t1"};
        
        leftmost(t1_cpy);
        rightmost(t1_cpy);
        leftmost(t2_cpy);
        rightmost(t2_cpy);
        t1_cpy.assert_properties_ok(sc);
        t2_cpy.assert_properties_ok(sc);
    }

    template <typename Tree, typename Vec>
    void lower_bound(Tree& tree, Vec& vals) {
        using namespace trbt::impl;
        using compare = typename Tree::key_compare;
        for(auto const& v : vals) {
            auto it = tree.lower_bound(v);

            for(auto tree_it = std::begin(tree); tree_it != std::end(tree); ++tree_it) {
                if(!compare{}(*tree_it, v)) {
                    if(it != tree_it)
                        throw value_retention_exception{"Lower bound doesn't return iter to proper lower bound"};
                    break;
                }
            }
        }
    }

    template <typename Tree, typename Vec>
    void upper_bound(Tree& tree, Vec& vals) {
        using namespace trbt::impl;
        using compare = typename Tree::key_compare;
        for(auto const& v : vals) {
            auto it = tree.upper_bound(v);

            for(auto tree_it = std::begin(tree); tree_it != std::end(tree); ++tree_it) {
                if(compare{}(v, *tree_it)) {
                    if(it != tree_it)
                        throw value_retention_exception{"Upper bound doesn't return iter to proper upper bound"};
                    break;
                }
            }
        }
    }
        

    template <typename Tree>
    void leftmost(Tree const& tree) {
        using namespace trbt::impl;

        int flags = tree.assert_leftmost_ok();

        if(flags) {
            std::ostringstream ss;
            ss << "Assertion of leftmost_ encountered the following issues:\n";
            
            if(bit_is_set(flags, THREAD_ERROR))
                ss << "- thread not set\n";
            if(bit_is_set(flags, LINK_ERROR))
                ss << "- left thread doesn't point to sentinel_\n";
            if(bit_is_set(flags, NODE_ERROR))
                ss << "- leftmost_ is not the leftmost node in the tree\n";
    
            throw thread_link_exception{ss.str()};
        }
    }

    template <typename Tree>
    void rightmost(Tree const& tree) {
        using namespace trbt::impl;

        int flags = tree.assert_rightmost_ok();

        if(flags) {
            std::ostringstream ss;
            ss << "Assertion of rightmost_ encountered the following issues:\n";
            
            if(bit_is_set(flags, THREAD_ERROR))
                ss << "- thread not set\n";
            if(bit_is_set(flags, LINK_ERROR))
                ss << "- left thread doesn't point to sentinel_\n";
            if(bit_is_set(flags, NODE_ERROR))
                ss << "- rightmost_ is not the rightmost node in the tree\n";
    
            throw thread_link_exception{ss.str()};
        }
    }

    template <typename Tree, typename T, typename StringConverter>
    void insert(Tree& tree, std::vector<T> const& vals, StringConverter sc) {
        using namespace trbt::impl;
        tree.clear();

        for(auto const& v : vals) {
            trace_insert_if_available(tree, v, TRACE_CALL_RESOLVER);
            tree.assert_properties_ok(sc);
            leftmost(tree);
            rightmost(tree);
        }
        if(tree.size() != vals.size()) {
            throw value_retention_exception{"Sizes differ. Tree: " + 
                    std::to_string(tree.size()) + " Vec: " + std::to_string(vals.size()) +
                    "\n"};
        }
    }

    template <typename Tree, typename T, typename StringConverter>
    void insert_range(Tree& tree, std::vector<T> const& vals, StringConverter sc) {
        using namespace trbt::impl;
        tree.clear();

        trace_insert_if_available(tree, std::begin(vals), std::end(vals), TRACE_CALL_RESOLVER);
        tree.assert_properties_ok(sc);
        leftmost(tree);
        rightmost(tree);
        if(tree.size() != vals.size()) {
            throw value_retention_exception{"Sizes differ. Tree: " + 
                    std::to_string(tree.size()) + " Vec: " + std::to_string(vals.size()) +
                    "\n"};
        }
    }

    template <typename Tree, typename T, typename StringConverter>
    void hinted_insert(Tree& tree, std::vector<T> const& vals, StringConverter sc) {
        using namespace trbt::impl;
        tree.clear();

        for(auto const& v : vals) {
            auto it = tree.upper_bound(v);
            trace_insert_if_available(tree, it, v, TRACE_CALL_RESOLVER);
            tree.assert_properties_ok(sc);
            leftmost(tree);
            rightmost(tree);
        }
        if(tree.size() != vals.size()) {
            throw value_retention_exception{"Sizes differ. Tree: " + 
                    std::to_string(tree.size()) + " Vec: " + std::to_string(vals.size()) +
                    "\n"};
        }
    }

    template <typename Tree, typename T, typename StringConverter>
    void emplace(Tree& tree, std::vector<T> const& vals, StringConverter sc) {
        using namespace trbt::impl;
        tree.clear();

        for(auto const& v : vals) {
            trace_emplace_if_available(tree, TRACE_CALL_RESOLVER, v);
            tree.assert_properties_ok(sc);
            leftmost(tree);
            rightmost(tree);
        }
        if(tree.size() != vals.size()) {
            throw value_retention_exception{"Sizes differ. Tree: " + 
                    std::to_string(tree.size()) + " Vec: " + std::to_string(vals.size()) +
                    "\n"};
        }
    }

    template <typename Tree, typename T, typename StringConverter>
    void hinted_emplace(Tree& tree, std::vector<T> const& vals, StringConverter sc) {
        using namespace trbt::impl;
        tree.clear();

        for(auto const& v : vals) {
            auto it = tree.upper_bound(v);
            trace_emplace_hint_if_available(tree, TRACE_CALL_RESOLVER, it, v);
            tree.assert_properties_ok(sc);
            leftmost(tree);
            rightmost(tree);
        }
        if(tree.size() != vals.size()) {
            throw value_retention_exception{"Sizes differ. Tree: " + 
                    std::to_string(tree.size()) + " Vec: " + std::to_string(vals.size()) +
                    "\n"};
        }
    }

    template <typename Tree, typename T, typename StringConverter>
    void pair_emplace(Tree& tree, std::vector<T> const& vals, StringConverter sc) {
        using namespace trbt::impl;
        tree.clear();

        for(auto const& v : vals) {
            trace_emplace_if_available(tree, TRACE_CALL_RESOLVER, v, double{});
            tree.assert_properties_ok(sc);
            leftmost(tree);
            rightmost(tree);
        }
        if(tree.size() != vals.size()) {
            throw value_retention_exception{"Sizes differ. Tree: " + 
                    std::to_string(tree.size()) + " Vec: " + std::to_string(vals.size()) +
                    "\n"};
        }
    }

    template <typename Tree, typename T, typename StringConverter>
    void pair_hinted_emplace(Tree& tree, std::vector<T> const& vals, StringConverter sc) {
        using namespace trbt::impl;
        tree.clear();

        for(auto const& v : vals) {
            std::pair<int, double> p{v, double{}};
            auto it = tree.upper_bound(p);
            trace_emplace_hint_if_available(tree, TRACE_CALL_RESOLVER, it, v, double{});
            tree.assert_properties_ok(sc);
            leftmost(tree);
            rightmost(tree);
        }
        if(tree.size() != vals.size()) {
            throw value_retention_exception{"Sizes differ. Tree: " + 
                    std::to_string(tree.size()) + " Vec: " + std::to_string(vals.size()) +
                    "\n"};
        }
    }

    template <typename Tree, typename Vec, typename StringConverter>
    void erase(Tree& tree, Vec& vals, StringConverter sc) {
        using namespace trbt::impl;
        std::mt19937 mt{std::random_device{}()};
        std::shuffle(std::begin(vals), std::end(vals), mt);
        
        for(int i = vals.size() - 1; i >= 0; --i) {
            if(!trace_erase_if_available(tree, vals[i], TRACE_CALL_RESOLVER))
                throw value_retention_exception{"Value to erase not in tree\n"};
            if(tree.erase(vals[i]))
                throw value_retention_exception{"Value still in tree after erasure\n"};
            vals.erase(std::end(vals) - 1, std::end(vals));
            
            tree.assert_properties_ok(sc);
            leftmost(tree);
            rightmost(tree);


            contains(tree, vals, sc);
        }

    }

    template <typename Tree, typename Vec>
    void eq(Tree& tree, Vec& vals) {
        using namespace trbt::impl;
        tree.clear();
        tree.insert(std::begin(vals), std::end(vals));
        Tree t;
        if(tree == t)
            throw value_retention_exception{"tree is equal to empty tree\n"};
        t = tree;
        if(!(tree == t))
            throw value_retention_exception{"Trees aren't equal after copy assignment\n"};
        
        t.erase(*std::begin(t));
        if(tree == t)
            throw value_retention_exception{"Trees are equals after deleting *begin\n"};
    }

    template <typename Tree, typename Vec>
    void neq(Tree& tree, Vec& vals) {
        using namespace trbt::impl;
        tree.clear();
        tree.insert(std::begin(vals), std::end(vals));
        Tree t;
        if(!(tree != t))
            throw value_retention_exception{"tree is equal to empty tree\n"};
        t = tree;
        if(tree != t)
            throw value_retention_exception{"Trees aren't equal after copy assignment\n"};
        
        t.erase(*std::begin(t));
        if(!(tree != t))
            throw value_retention_exception{"Trees are equals after deleting *begin\n"};
    }

    template <typename Tree, typename Vec, typename Incrementor>
    void less(Tree& tree, Vec const& vals, Incrementor inc) {
        using namespace trbt::impl;
        Tree ltree;
        tree.clear();

        auto i = 0u;
        for(; i < vals.size() - 1u; i++) {
            tree.insert(vals[i]);
            ltree.insert(vals[i]);
        }
        tree.insert(vals[i]);
        ltree.insert(inc(vals[i]));
        
        if(!(tree < ltree))
            throw ordering_exception{"Operator< yields wrong result when incrementing last\n"};

        tree.clear();
        ltree.clear();

        tree.insert(vals[0]);
        ltree.insert(inc(vals[0]));
        i = 1u;
        for(; i < vals.size(); i++) {
            tree.insert(vals[i]);
            ltree.insert(vals[i]);
        }

        if(!(tree < ltree))
            throw ordering_exception{"Operator< yields wrong result when incrementing first\n"};

        tree.clear();
        ltree.clear();

        for(i = 0u; i < vals.size() / 2u; i++) {
            tree.insert(vals[i]);
            ltree.insert(vals[i]);
        }

        tree.insert(vals[i]);
        ltree.insert(inc(vals[i++]));

        for(; i < vals.size(); i++) {
            tree.insert(vals[i]);
            ltree.insert(vals[i]);
        }

        if(!(tree < ltree))
            throw ordering_exception{"Operator< yields wrong result when incrementing in the middle\n"};

        tree.clear();
        ltree.clear();
        
        for(i = 0u; i < vals.size() - 1; i++) {
            tree.insert(vals[i]);
            ltree.insert(vals[i]);
        }
        ltree.insert(vals[i]);

        if(!(tree < ltree))
            throw ordering_exception{"Operator< yields wrong result when trees are different sizes\n"};
    }

    template <typename Tree, typename Vec, typename Incrementor>
    void greater(Tree& tree, Vec const& vals, Incrementor inc) {
        using namespace trbt::impl;
        Tree ltree;
        tree.clear();

        auto i = 0u;
        for(; i < vals.size() - 1u; i++) {
            tree.insert(vals[i]);
            ltree.insert(vals[i]);
        }
        tree.insert(vals[i]);
        ltree.insert(inc(vals[i]));
        
        if(!(ltree > tree))
            throw ordering_exception{"Operator> yields wrong result when incrementing last\n"};

        tree.clear();
        ltree.clear();

        tree.insert(vals[0]);
        ltree.insert(inc(vals[0]));
        i = 1u;
        for(; i < vals.size(); i++) {
            tree.insert(vals[i]);
            ltree.insert(vals[i]);
        }

        if(!(ltree > tree))
            throw ordering_exception{"Operator> yields wrong result when incrementing first\n"};

        tree.clear();
        ltree.clear();

        for(i = 0u; i < vals.size() / 2u; i++) {
            tree.insert(vals[i]);
            ltree.insert(vals[i]);
        }

        tree.insert(vals[i]);
        ltree.insert(inc(vals[i++]));

        for(; i < vals.size(); i++) {
            tree.insert(vals[i]);
            ltree.insert(vals[i]);
        }

        if(!(ltree > tree))
            throw ordering_exception{"Operator> yields wrong result when incrementing in middle\n"};

        tree.clear();
        ltree.clear();
        
        for(i = 0u; i < vals.size() - 1; i++) {
            tree.insert(vals[i]);
            ltree.insert(vals[i]);
        }
        ltree.insert(vals[i]);

        if(!(ltree > tree))
            throw ordering_exception{"Operator> yields wrong result when trees are different sizes\n"};
    }

    template <typename Tree, typename Vec, typename Incrementor>
    void less_or_eq(Tree& tree, Vec const& vals, Incrementor inc) {
        using namespace trbt::impl;
        Tree ltree(tree);
        
        if(!(tree <= ltree))
            throw ordering_exception{"Operator<= returns wrong when equal\n"};

        tree.clear();
        ltree.clear();

        auto i = 0u;
        for(; i < vals.size() - 1u; i++) {
            tree.insert(vals[i]);
            ltree.insert(vals[i]);
        }
        tree.insert(vals[i]);
        ltree.insert(inc(vals[i]));
        
        if(!(tree <= ltree))
            throw ordering_exception{"Operator<= yields wrong result when incrementing last\n"};

        tree.clear();
        ltree.clear();

        tree.insert(vals[0]);
        ltree.insert(inc(vals[0]));
        i = 1u;
        for(; i < vals.size(); i++) {
            tree.insert(vals[i]);
            ltree.insert(vals[i]);
        }

        if(!(tree <= ltree))
            throw ordering_exception{"Operator<= yields wrong result when incrementing first\n"};

        tree.clear();
        ltree.clear();

        for(i = 0u; i < vals.size() / 2u; i++) {
            tree.insert(vals[i]);
            ltree.insert(vals[i]);
        }

        tree.insert(vals[i]);
        ltree.insert(inc(vals[i++]));

        for(; i < vals.size(); i++) {
            tree.insert(vals[i]);
            ltree.insert(vals[i]);
        }

        if(!(tree <= ltree))
            throw ordering_exception{"Operator<= yields wrong result when incrementing in middle\n"};

        tree.clear();
        ltree.clear();
        
        for(i = 0u; i < vals.size() - 1; i++) {
            tree.insert(vals[i]);
            ltree.insert(vals[i]);
        }
        ltree.insert(vals[i]);

        if(!(tree <= ltree))
            throw ordering_exception{"Operator<= yields wrong result when trees are different sizes\n"};
    }

    template <typename Tree, typename Vec, typename Incrementor>
    void greater_or_eq(Tree& tree, Vec const& vals, Incrementor inc) {
        using namespace trbt::impl;
        Tree ltree(tree);
        if(!(ltree >= tree))
            throw ordering_exception{"Operator>= yields wrong result when equal\n"};

        tree.clear();
        ltree.clear();

        auto i = 0u;
        for(; i < vals.size() - 1u; i++) {
            tree.insert(vals[i]);
            ltree.insert(vals[i]);
        }
        tree.insert(vals[i]);
        ltree.insert(inc(vals[i]));
        
        if(!(ltree >= tree))
            throw ordering_exception{"Operator>= yields wrong result when incrementing last\n"};

        tree.clear();
        ltree.clear();

        tree.insert(vals[0]);
        ltree.insert(inc(vals[0]));
        i = 1u;
        for(; i < vals.size(); i++) {
            tree.insert(vals[i]);
            ltree.insert(vals[i]);
        }

        if(!(ltree >= tree))
            throw ordering_exception{"Operator>= yields wrong result when incrementing first\n"};

        tree.clear();
        ltree.clear();

        for(i = 0u; i < vals.size() / 2u; i++) {
            tree.insert(vals[i]);
            ltree.insert(vals[i]);
        }

        tree.insert(vals[i]);
        ltree.insert(inc(vals[i++]));

        for(; i < vals.size(); i++) {
            tree.insert(vals[i]);
            ltree.insert(vals[i]);
        }

        if(!(ltree >= tree))
            throw ordering_exception{"Operator>= yields wrong result when incrementing in middle\n"};

        tree.clear();
        ltree.clear();
        
        for(i = 0u; i < vals.size() - 1; i++) {
            tree.insert(vals[i]);
            ltree.insert(vals[i]);
        }
        ltree.insert(vals[i]);

        if(!(ltree >= tree))
            throw ordering_exception{"Operator>= yields wrong result when trees are different sizes\n"};
    }


    template <typename Tree, typename Vec>
    void iters(Tree& tree, Vec& vals) {
        using namespace trbt::impl;
        using iter    = typename Tree::iterator;
        using c_iter  = typename Tree::const_iterator;
        using r_iter  = typename Tree::reverse_iterator;
        using cr_iter = typename Tree::const_reverse_iterator;
        
        bool constexpr is_pair = is_pair_v<remove_cvref_t<decltype(*std::declval<iter>())>>;

        if constexpr(is_pair) {
            static_assert(!is_assignable_v<decltype(*std::declval<iter>())>);
            static_assert(!is_assignable_v<decltype(*std::declval<r_iter>())>);
        }
        else {
            static_assert(is_assignable_v<decltype(*std::declval<iter>())>);
            static_assert(is_assignable_v<decltype(*std::declval<r_iter>())>);
        }
        static_assert(!is_assignable_v<decltype(*std::declval<c_iter>())>);
        static_assert(!is_assignable_v<decltype(*std::declval<cr_iter>())>);

        auto not_equal = [](auto const& left, auto const& right) {
            if constexpr(is_pair) 
                return left.first != right.first || left.second != right.second;
            else
                return left != right;
        };
        
        tree.clear();
        tree.insert(std::begin(vals), std::end(vals));
        std::sort(std::begin(vals), std::end(vals));
        
        {
            auto tree_it = std::begin(tree);
            auto vec_it  = std::begin(vals);

            for(; tree_it != std::end(tree) && vec_it != std::end(vals); ++tree_it, ++vec_it) {
                if(not_equal(*tree_it, *vec_it))
                    throw iterator_exception{"Iterator dereferencing yields different values\n"};
            }

            if(tree_it != std::end(tree))
                throw iterator_exception{"Tree iterator is not end\n"};
            if(vec_it != std::end(vals))
                throw iterator_exception{"Vec iterator is not end\n"};
        }
        {
            auto tree_it = std::rbegin(tree);
            auto vec_it  = std::rbegin(vals);
            
            for(; tree_it != std::rend(tree) && vec_it != std::rend(vals); ++tree_it, ++vec_it) {
                if(not_equal(*tree_it, *vec_it))
                    throw iterator_exception{"Reverse iterator dereferencing yields different values\n"};
            }

            if(tree_it != std::rend(tree))
                throw iterator_exception{"Tree reverse iterator is not end\n"};
            if(vec_it != std::rend(vals))
                throw iterator_exception{"Vec reverse iterator is not end\n"};
        }
        {
            auto tree_it = std::cbegin(tree);
            auto vec_it  = std::cbegin(vals);
            
            for(; tree_it != std::cend(tree) && vec_it != std::cend(vals); ++tree_it, ++vec_it) {
                if(not_equal(*tree_it, *vec_it))
                    throw iterator_exception{"Const iterator derefernecing yields different values\n"};
            }
            if(tree_it != std::cend(tree))
                throw iterator_exception{"Tree reverse iterator is not end\n"};
            if(vec_it != std::cend(vals))
                throw iterator_exception{"Vec reverse iterator is not end\n"};
        }
        {
            auto tree_it = std::crbegin(tree);
            auto vec_it  = std::crbegin(vals);

            for(; tree_it != std::crend(tree) && vec_it != std::crend(vals); ++tree_it, ++vec_it) {
                if(not_equal(*tree_it, *vec_it))
                    throw iterator_exception{"Const reverse iterator dereferencing yields differnet values\n"};
            }
            if(tree_it != std::crend(tree))
                throw iterator_exception{"Tree reverse iterator is not end\n"};
            if(vec_it != std::crend(vals))
                throw iterator_exception{"Vec reverse iterator is not end\n"};
        }
        auto it = std::begin(vals);
        for(auto const& v : tree) {
            if(not_equal(v, *it++))
                throw iterator_exception{"Range based for yields different values\n"};
        }
        if(std::begin(tree) == std::end(tree))
            throw iterator_exception{"begin equals end when not empty\n"};
        if(std::cbegin(tree) == std::cend(tree))
            throw iterator_exception{"cbegin equals cend when not empty\n"};
        if(std::rbegin(tree) == std::rend(tree))
            throw iterator_exception{"rbegin equals rend when not empty\n"};
        if(std::crbegin(tree) == std::crend(tree))
            throw iterator_exception{"crbegin equals crend when not empty\n"};

        tree.clear();

        if(std::begin(tree) != std::end(tree))
            throw iterator_exception{"begin doesn't equal end after call to clear\n"};
        if(std::cbegin(tree) != std::cend(tree))
            throw iterator_exception{"cbegin doesn't equal cend after call to clear\n"};
        if(std::rbegin(tree) != std::rend(tree))
            throw iterator_exception{"rbegin doesn't equal rend after call to clear\n"};
        if(std::crbegin(tree) != std::crend(tree))
            throw iterator_exception{"crbegin doesn't equal crend after call to clear\n"};

        tree.insert(std::begin(vals), std::end(vals));
        for(auto const& v : vals)
            tree.erase(v);

        if(std::begin(tree) != std::end(tree))
            throw iterator_exception{"begin doesn't equal end after erasing all values\n"};
        if(std::cbegin(tree) != std::cend(tree))
            throw iterator_exception{"cbegin doesn't equal cend after erasing all values\n"};
        if(std::rbegin(tree) != std::rend(tree))
            throw iterator_exception{"rbegin doesn't equal rend after erasing all values\n"};
        if(std::crbegin(tree) != std::crend(tree))
            throw iterator_exception{"crbegin doesn't equal crend after erasing all values\n"};
    }
    
} /* namespace test */
} /* namespace trbt */


#endif
