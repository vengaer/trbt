#ifndef TRBT_TRACE_TYPE_H
#define TRBT_TRACE_TYPE_H

#pragma once
#include "trbt.h"
#include "trbt_test_config.h"
#include <iomanip>
#include <iostream>
#include <deque>

namespace trbt {
namespace impl {

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
    
} /* namespace impl */

namespace test {

template <typename Tree>
struct trbt_trace_type : public Tree {

    using value_type = typename Tree::value_type;
    using iterator = typename Tree::iterator;
    using const_iterator = typename Tree::const_iterator;
    using size_type = typename Tree::size_type;

    using Tree::Tree;

    template <typename T = value_type, typename = impl::enable_if_convertible_t<T, value_type>>
    std::pair<iterator, bool> traced_insert(T&& value) {
        add_trace();
        return Tree::insert(std::forward<T>(value));
    }

    template <typename InputIt, typename = impl::enable_if_iterator_t<InputIt>>
    void traced_insert(InputIt first, InputIt last) {
        add_trace();
        Tree::insert(first, last);
    }

    template <typename T = value_type, typename = impl::enable_if_convertible_t<T, value_type>>
    iterator traced_insert(const_iterator hint, T&& value) {
        add_trace();
        return Tree::insert(hint, std::forward<T>(value));
    }

    template <typename... Args>
    std::pair<iterator, bool> traced_emplace(Args&&... args) {
        add_trace();
        return Tree::emplace(std::forward<Args>(args)...);
    }

    template <typename... Args>
    iterator traced_emplace_hint(const_iterator hint, Args&&... args) {
        add_trace();
        return Tree::emplace_hint(hint, std::forward<Args>(args)...);
    }

    size_type traced_erase(value_type const& value) {
        add_trace();
        return Tree::erase(value);
    }

    void add_trace() {
        trace.push_back(*this);

        if(trace.size() > trbt_trace_size)
            trace.pop_front();
    }

    void clear_trace() {
        trace.clear();
    }

    void print_trace(std::ostream& os = std::cout) {
        os << std::setfill('-') << std::setw(30) << "" 
           << "\nPrinting trace with size " << trace.size() << "\n\n";
        for(auto i = 0u; i < trace.size(); i++) {
            os << std::setfill('+') << std::setw(30) << "" << "\ntrace number: "
               << i << "\n" << "size: " << trace[i].size() << "\n" 
               << std::setw(30) << "" << "\n" << std::setfill(' ');
            trace[i].print(os);
        }
        os << std::setfill('+') << std::setw(30) << "" << "\nCurrent\n"
           << "size: " << this->size() << "\n" << std::setw(30) << "" << "\n" 
           << std::setfill(' ');
        this->print(os);
        os << std::setfill(' ');
    }

    bool active{false};
    std::deque<Tree> trace{};

};

inline int constexpr TRACE_CALL_RESOLVER{0};

/* Drivers for tracing operations. Fall back on non-traced
 * versions if Tree is not generated from  trbt_trace_type */

template <typename Tree, typename T>
auto trace_insert_if_available(Tree& tree, T&& value, int) 
    -> std::remove_reference_t< decltype(tree.traced_insert(std::forward<T>(value)))>
                      
{
    return tree.traced_insert(std::forward<T>(value));
}

template <typename Tree, typename T>
auto trace_insert_if_available(Tree& tree, T&& value, long) 
    -> std::pair<typename Tree::iterator, bool>
{
    return tree.insert(std::forward<T>(value));
}

template <typename Tree, typename InputIt>
auto trace_insert_if_available(Tree& tree, InputIt first, InputIt last, int) 
    -> decltype((void)tree.traced_insert(first, last)) 
{
    tree.traced_insert(first, last);
}

template <typename Tree, typename InputIt>
auto trace_insert_if_available(Tree& tree, InputIt first, InputIt last, long) -> void {
    tree.insert(first, last);
}

template <typename Tree, typename T>
auto trace_insert_if_available(Tree& tree, typename Tree::const_iterator hint, T&& value, int) 
    -> std::remove_reference_t< decltype(tree.traced_insert(hint, std::forward<T>(value)))>
                
{
    return tree.traced_insert(hint, std::forward<T>(value));
}

template <typename Tree, typename T>
auto trace_insert_if_available(Tree& tree, typename Tree::const_iterator hint, T&& value, long) 
    -> typename Tree::iterator 
{
    return tree.insert(hint, std::forward<T>(value));
}

template <typename Tree, typename... Args>
auto trace_emplace_if_available(Tree& tree, int, Args&&... args) 
    -> std::remove_reference_t<decltype(tree.traced_emplace(std::forward<Args>(args)...))>
{
    return tree.traced_emplace(std::forward<Args>(args)...);
}

template <typename Tree, typename... Args>
auto trace_emplace_if_available(Tree& tree, long, Args&&... args) 
    ->  std::pair<typename Tree::iterator, bool>
{
    return tree.emplace(std::forward<Args>(args)...);
}

template <typename Tree, typename... Args>
auto trace_emplace_hint_if_available(Tree& tree, int, 
        typename Tree::const_iterator hint, Args&&... args)
                -> std::remove_reference_t<
                       decltype(tree.emplace_hint(hint, std::forward<Args>(args)...))>
{
    return tree.traced_emplace_hint(hint, std::forward<Args>(args)...);
}

template <typename Tree, typename... Args>
auto trace_emplace_hint_if_available(Tree& tree, long, 
        typename Tree::const_iterator hint, Args&&... args) -> typename Tree::iterator
{
    return tree.emplace_hint(hint, std::forward<Args>(args)...);
}

template <typename Tree>
auto trace_erase_if_available(Tree& tree, typename Tree::value_type const& value, int)
    -> std::remove_reference_t<decltype(tree.traced_erase(value))>
{
    return tree.traced_erase(value);
}

template <typename Tree>
auto trace_erase_if_available(Tree& tree, typename Tree::value_type const& value, long) 
    -> typename Tree::size_type
{
    return tree.erase(value);
}


} /* namespace test */
} /* namespace trbt */


#endif
