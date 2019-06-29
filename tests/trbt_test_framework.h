#ifndef TRBT_TEST_FRAMEWORK_H
#define TRBT_TEST_FRAMEWORK_H

#define TRBT_DEBUG
#pragma once
#include "trbt.h"
#include <array>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <utility>
#include <vector>


namespace trbt {

namespace test {
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

    template <typename StringConverter>
    void pair_emplace(rbtree<std::pair<int, double>>& tree, std::vector<int> const& vals, StringConverter sc);

    template <typename StringConverter>
    void pair_hinted_emplace(rbtree<std::pair<int, double>>& tree, std::vector<int> const& vals, StringConverter sc);

    template <typename Tree, typename Vec, typename StringConverter>
    void contains(Tree const& tree, Vec const& vals, StringConverter sc);

    template <typename Tree, typename Vec, typename StringConverter>
    void erase(Tree& tree, Vec& vals, StringConverter sc);

    template <typename Tree>
    void empty(Tree const& tree);

    std::vector<int> generate_int_vec(std::size_t size);
    std::vector<std::string> generate_string_vec(std::size_t size);
    std::vector<std::pair<int, double>> generate_pair_vec(std::size_t size);

    void print_heading(std::string const& method, std::size_t test_size, std::size_t current_iter, std::size_t total_iters);

    /* Definitions */

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
        for(auto const& v : vals) {
            tree.insert(v);
            tree.assert_properties_ok(sc);
            leftmost(tree);
            rightmost(tree);
        }
    }

    template <typename Tree, typename T, typename StringConverter>
    void insert_range(Tree& tree, std::vector<T> const& vals, StringConverter sc) {
        tree.insert(std::begin(vals), std::end(vals));
        tree.assert_properties_ok(sc);
        leftmost(tree);
        rightmost(tree);
    }

    template <typename Tree, typename T, typename StringConverter>
    void hinted_insert(Tree& tree, std::vector<T> const& vals, StringConverter sc) {
        for(auto const& v : vals) {
            auto it = tree.upper_bound(v);
            tree.insert(it, v);
            tree.assert_properties_ok(sc);
            leftmost(tree);
            rightmost(tree);
        }
    }

    template <typename Tree, typename T, typename StringConverter>
    void emplace(Tree& tree, std::vector<T> const& vals, StringConverter sc) {
        for(auto const& v : vals) {
            tree.emplace(v);
            tree.assert_properties_ok(sc);
            leftmost(tree);
            rightmost(tree);
        }
    }

    template <typename Tree, typename T, typename StringConverter>
    void hinted_emplace(Tree& tree, std::vector<T> const& vals, StringConverter sc) {
        for(auto const& v : vals) {
            auto it = tree.upper_bound(v);
            tree.emplace_hint(it, v);
            tree.assert_properties_ok(sc);
            leftmost(tree);
            rightmost(tree);
        }
    }

    template <typename StringConverter>
    void pair_emplace(rbtree<std::pair<int, double>>& tree, std::vector<int> const& vals, StringConverter sc) {
        for(auto const& v : vals) {
            tree.emplace(v, double{});
            tree.assert_properties_ok(sc);
            leftmost(tree);
            rightmost(tree);
        }
    }

    template <typename StringConverter>
    void pair_hinted_emplace(rbtree<std::pair<int, double>>& tree, std::vector<int> const& vals, StringConverter sc) {
        for(auto const& v : vals) {
            std::pair<int, double> p{v, double{}};
            auto it = tree.upper_bound(p);
            tree.emplace_hint(it, v, double{});
            tree.assert_properties_ok(sc);
            leftmost(tree);
            rightmost(tree);
        }
    }

    template <typename Tree, typename Vec, typename StringConverter>
    void contains(Tree const& tree, Vec const& vals, StringConverter sc) {
        using namespace trbt::impl;
        for(auto const& v : vals) {
            if(!tree.contains(v))
                throw value_retention_exception{sc(v) + " not in tree\n"};
        }
    }

    template <typename Tree, typename Vec, typename StringConverter>
    void erase(Tree& tree, Vec& vals, StringConverter sc) {
        std::mt19937 mt{std::random_device{}()};
        
        while(vals.size()) {
            std::uniform_int_distribution<> dis(0, vals.size() - 1);
            auto val = vals[dis(mt)];

            tree.erase(val);
            tree.assert_properties_ok(sc);
            leftmost(tree);
            rightmost(tree);

            vals.erase(std::remove_if(std::begin(vals), std::end(vals), [val](auto const& v) {
                return v == val;
            }), std::end(vals));

            contains(tree, vals, sc);
        }
    }
    
    template <typename Tree>
    void empty(Tree const& tree) {
        using namespace impl::debug;

        if(!tree.empty())
            throw value_retention_exception{"Tree is not empty\n"};
    }
} /* namespace test */
} /* namespace trbt */


#endif
