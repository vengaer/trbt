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
            : state{var} { 
            state = target_state;
        }

        ~scoped_bool() {
            state = !state;
        }

        bool get() const {
            return state;
        }

    private:
        bool& state;
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
    std::pair<iterator, bool> insert(T&& value) {
        add_trace();
        return Tree::insert(std::forward<T>(value));
    }

    template <typename InputIt, typename = impl::enable_if_iterator_t<InputIt>>
    void insert(InputIt first, InputIt last) {
        add_trace();
        Tree::insert(first, last);
    }

    template <typename T = value_type, typename = impl::enable_if_convertible_t<T, value_type>>
    iterator insert(const_iterator hint, T&& value) {
        add_trace();
        return Tree::insert(hint, std::forward<T>(value));
    }

    template <typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        add_trace();
        return Tree::emplace(std::forward<Args>(args)...);
    }

    template <typename... Args>
    iterator emplace_hint(const_iterator hint, Args&&... args) {
        add_trace();
        return Tree::emplace_hint(hint, std::forward<Args>(args)...);
    }

    size_type erase(value_type const& value) {
        add_trace();
        return Tree::erase(value);
    }

    void add_trace() {
        trace.push_back(*this);

        if(trace.size() >= trbt_trace_size)
            trace.pop_front();
    }

    void clear_trace() {
        trace.clear();
    }

    void print_trace(std::ostream& os = std::cout) {
        os << std::setfill('-') << std::setw(30) << "" 
           << "\nPrinting trace with size " << trace.size() << "\n\n";
        for(auto i = 0u; i < trace.size(); i++) {
            os << std::setfill('*') << std::setw(30) << "" << "\ntrace number: "
               << i << "\n" << std::setw(30) << "" << "\n" 
               << "size: " << trace[i].size() << "\n";
            trace[i].print(os);
        }
        os << std::setfill('*') << std::setw(30) << "" << "\nCurrent\n"
           << std::setw(30) << "" << "\n" << "size: " << this->size() << "\n";
        this->print(os);
        os << std::setfill(' ');
    }

    std::deque<Tree> trace{};
    bool active{false};
};

} /* namespace test */
} /* namespace trbt */


#endif
