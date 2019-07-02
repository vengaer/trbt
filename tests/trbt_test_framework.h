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
#include <string>
#include <utility>
#include <vector>


namespace trbt {

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
    
} /* namespace test */
} /* namespace trbt */


#endif
