#ifndef TRBT_H
#define TRBT_H

#define TRBT_TYPE_LIST template <typename Value, typename Compare, typename Allocator, typename Void>
#define TRBT_MEM_SPEC red_black_tree<Value, Compare, Allocator, Void>::
#define TRBT_NODE_PTR typename TRBT_MEM_SPEC node_type*
#define TRBT_SIZE_TYPE typename TRBT_MEM_SPEC size_type
#define TRBT_ITER_TYPE typename TRBT_MEM_SPEC iterator
#define TRBT_CONST_ITER_TYPE typename TRBT_MEM_SPEC const_iterator
#define TRBT_REVERSE_ITER_TYPE typename TRBT_MEM_SPEC reverse_iterator
#define TRBT_CONST_REVERSE_ITER_TYPE typename TRBT_MEM_SPEC const_reverse_iterator
#define TRBT_RED_BLACK_TREE_REF red_black_tree<Value, Compare, Allocator, Void>&
#define TRBT_ALLOCATOR_TYPE typename TRBT_MEM_SPEC allocator_type
#define TRBT_INSERT_RETURN_PAIR std::pair<TRBT_ITER_TYPE, bool>

#define TRBT_MAP_TYPE_LIST template <typename Pair, typename Compare, typename Allocator>
#define TRBT_MAP_MEM_SPEC red_black_tree<Pair, Compare, Allocator, impl::enable_if_pair<Pair>>::
#define TRBT_MAP_NODE_PTR typename TRBT_MAP_MEM_SPEC node_type*
#define TRBT_MAP_SIZE_TYPE typename TRBT_MAP_MEM_SPEC size_type
#define TRBT_MAP_ITER_TYPE typename TRBT_MAP_MEM_SPEC iterator
#define TRBT_MAP_CONST_ITER_TYPE typename TRBT_MAP_MEM_SPEC const_iterator
#define TRBT_MAP_REVERSE_ITER_TYPE typename TRBT_MAP_MEM_SPEC reverse_iterator
#define TRBT_MAP_CONST_REVERSE_ITER_TYPE typename TRBT_MAP_MEM_SPEC const_reverse_iterator
#define TRBT_MAP_RED_BLACK_TREE_REF red_black_tree<Pair, Compare, Allocator, impl::enable_if_pair<Pair>>&
#define TRBT_MAP_ALLOCATOR_TYPE typename TRBT_MAP_MEM_SPEC allocator_type
#define TRBT_MAP_INSERT_RETURN_PAIR std::pair<TRBT_MAP_ITER_TYPE, bool>

#pragma once
#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

#ifdef TRBT_DEBUG
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>
#endif

namespace trbt {

namespace impl {
    template <typename, typename, typename = void>
    struct is_comparable : std::false_type { };

    template <typename Value, typename Compare>
    struct is_comparable<Value, 
                         Compare, 
                         std::enable_if_t<
                            std::is_same_v<
                                bool, 
                                decltype(std::declval<Compare>()(std::declval<Value>(), std::declval<Value>()))>>>
        : std::true_type{ };

    template <typename Value, typename Compare>
    inline bool constexpr is_comparable_v = is_comparable<Value, Compare>::value;

    template <typename T>
    using remove_cvref = std::remove_cv<std::remove_reference_t<T>>;

    template <typename T>
    using remove_cvref_t = typename remove_cvref<T>::type;

    template <typename T,typename U>
    using disable_if_same = std::enable_if_t<!std::is_same_v<remove_cvref_t<T>, U>>;

    template <typename, typename = void>
    struct satisfies_input_iterator : std::false_type { };

    template <typename T>
    struct satisfies_input_iterator<T, std::enable_if_t<
                                            std::is_base_of_v<
                                                std::input_iterator_tag, 
                                                typename std::iterator_traits<T>::iterator_category>>> 
        : std::true_type { };

    template <typename T>
    using enable_if_iterator = std::enable_if_t<satisfies_input_iterator<T>::value>;

    template <typename T, typename U>
    using enable_if_convertible = std::enable_if_t<std::is_convertible_v<T, U>>;
    
    template <typename T>
    struct type_is {
        using type = T;
    };

    template <typename T>
    struct compare_type : type_is<T> { };

    template <typename K, typename M>
    struct compare_type<std::pair<K, M>> : type_is<K> { };

    template <typename T>
    using compare_type_t = typename compare_type<T>::type;

    template <typename T>
    struct is_pair : std::false_type { };

    template <typename K, typename M>
    struct is_pair<std::pair<K, M>> : std::true_type { };

    template <typename T>
    inline bool constexpr is_pair_v = is_pair<T>::value;

    template <typename... Ts>
    using enable_if_pair = std::enable_if_t<(is_pair_v<Ts> && ...)>;

    enum class Color { Red, Black };
    enum class Direction { Left, Right };

    inline Direction operator!(Direction dir) {
        return static_cast<Direction>(!static_cast<std::underlying_type_t<Direction>>(dir));
    }

    template <typename Value>
    struct node {
        Value value;
        node *left, *right;
        Color color;
        unsigned char thread;
        
        template <typename T = Value, typename = disable_if_same<T, node>>
        node(T&& value = T(), node* l = nullptr, node* r = nullptr, Color col = Color::Black, unsigned char threaded = 0x0) 
            : value{std::forward<T>(value)}, left{l}, right{r}, color{col}, thread{threaded} { }

        bool is_leaf() const {
            return thread == (0x1 | 0x2);
        }
    
        bool has_left_child() const {
            return !(thread & 0x2);
        }

        bool has_right_child() const {
            return !(thread & 0x1);
        }
    
        void set_left_thread() {
            thread |= 0x2;
        }

        void set_right_thread() {
            thread |= 0x1;
        }

        void unset_left_thread() {
            thread &= ~0x2;
        }

        void unset_right_thread() {
            thread &= ~0x1;
        }

    };

    #ifdef TRBT_DEBUG
    namespace debug {
        struct color_violation_exception : std::runtime_error {
            using std::runtime_error::runtime_error;
        };
        struct bst_property_violation_exception : std::runtime_error {
            using std::runtime_error::runtime_error;
        };
        struct height_violation_exception : std::runtime_error {
            using std::runtime_error::runtime_error;
        };
        struct value_retention_exception : std::runtime_error {
            using std::runtime_error::runtime_error;
        };

    } /* namespace debug */
    #endif

    template <typename Container, typename Derived, bool is_reverse = false>
    class trbt_iterator_base {
        protected:
            using node_type = node<typename Container::value_type>;
        public:
            using value_type        = typename Container::value_type;
            using difference_type   = std::ptrdiff_t;
            using iterator_category = std::bidirectional_iterator_tag;

            explicit trbt_iterator_base(Container* cont) : parent_{cont}, current_{parent_->header_} { }
            trbt_iterator_base(Container* cont, node_type* t) : parent_{cont}, current_{t} { }

            Derived& operator=(trbt_iterator_base const& rhs) & {
                this->parent_ = rhs.parent_;
                this->current_ = rhs.current_;
                return static_cast<Derived&>(*this);
            }
        
            friend void swap(trbt_iterator_base& left, trbt_iterator_base& right) noexcept {
                auto temp = std::move(left);
                left = std::move(right);
                right = std::move(temp);
            }

            friend bool operator==(trbt_iterator_base const& left, trbt_iterator_base const& right) {
                return left.current_ == right.current_;
            }

            friend bool operator!=(trbt_iterator_base const& left, trbt_iterator_base const& right) {
                return !(left == right);
            }
    
            Derived& operator++() {
                if constexpr(is_reverse)
                    this->current_ = this->parent_->predecessor(this->current_);
                else
                    this->current_ = this->parent_->successor(this->current_);
                return static_cast<Derived&>(*this);
            }

            Derived operator++(int) {
                auto prev = static_cast<Derived&>(*this);
                ++*this;
                return prev;
            }

            Derived& operator--() {
                if constexpr(is_reverse)
                    this->current_ = this->parent_->successor(this->current_);
                else
                    this->current_ = this->parent_->predecessor(this->current_);
                return static_cast<Derived&>(*this);
            }

            Derived operator--(int) {
                auto next = static_cast<Derived&>(*this);
                --*this;
                return next;
            }

        private:
            Container* parent_;
        protected:
            node_type* current_;
    };

    template <typename Container, bool is_reverse>
    class trbt_iterator_type : public trbt_iterator_base<Container, trbt_iterator_type<Container, is_reverse>, is_reverse> {
        using base      = trbt_iterator_base<Container, trbt_iterator_type<Container, is_reverse>, is_reverse>;
        using node_type = typename base::node_type;
        public:
            using difference_type   = typename base::difference_type;
            using value_type        = typename base::value_type;
            using reference         = typename Container::reference;
            using const_reference   = typename Container::const_reference;
            using pointer           = typename Container::pointer;
            using const_pointer     = typename Container::const_pointer;
            using iterator_category = typename base::iterator_category;

            using base::base;

            reference operator*() {
                return this->current_->value;
            }
            
            const_reference operator*() const {
                return this->current_->value;
            }

            pointer operator->() {
                return std::addressof(this->current_->value);
            }
        
            const_pointer operator->() const {
                return std::addressof(this->current_->value);
            }
    };
    
    template <typename Container, bool is_reverse>
    class trbt_const_iterator_type : public trbt_iterator_base<Container const, trbt_const_iterator_type<Container const, is_reverse>, is_reverse> {
        using base      = trbt_iterator_base<Container const, trbt_const_iterator_type<Container const, is_reverse>, is_reverse>;
        using node_type = typename base::node_type;
        public:
            using difference_type   = typename base::difference_type;
            using value_type        = typename base::value_type;
            using reference         = typename Container::const_reference;
            using const_reference   = typename Container::const_reference;
            using pointer           = typename Container::const_pointer;
            using const_pointer     = typename Container::const_pointer;
            using iterator_category = typename base::iterator_category;

            using base::base;
            
            reference operator*() {
                return this->current_->value;
            }
            
            const_reference operator*() const {
                return this->current_->value;
            }

            pointer operator->() {
                return std::addressof(this->current_->value);
            }
        
            const_pointer operator->() const {
                return std::addressof(this->current_->value);
            }
    };

    template <typename Container>
    using iterator = trbt_iterator_type<Container, false>;
    template <typename Container>
    using reverse_iterator = trbt_iterator_type<Container, true>;

    template <typename Container>
    using const_iterator = trbt_const_iterator_type<Container, false>;
    template <typename Container>
    using const_reverse_iterator = trbt_const_iterator_type<Container, true>;

} /* namespace impl */

template <typename Value, 
          typename Compare = std::less<impl::compare_type_t<Value>>, 
          typename Allocator = std::allocator<Value>,
          typename = void> /* Used with SFINAE in order to detect map type */
class red_black_tree {
    static_assert(impl::is_comparable_v<impl::compare_type_t<Value>, Compare>, "Value type is not comparable");

    template <typename, typename, bool>
    friend class impl::trbt_iterator_base;

    using Alloc     = typename std::allocator_traits<Allocator>::template rebind_alloc<impl::node<Value>>;
    using Color     = impl::Color;
    using Direction = impl::Direction;

    #ifdef TRBT_DEBUG
    using color_violation_exception        = impl::debug::color_violation_exception;
    using bst_property_violation_exception = impl::debug::bst_property_violation_exception;
    using height_violation_exception       = impl::debug::height_violation_exception;
    #endif

    public:
        using key_type               = Value;
        using value_type             = Value;
        using size_type              = std::size_t;
        using difference_type        = std::ptrdiff_t;
        using key_compare            = Compare;
        using value_compare          = Compare;
        using allocator_type         = Allocator;
        using reference              = value_type&;
        using const_reference        = value_type const&;
        using pointer                = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer          = typename std::allocator_traits<Allocator>::const_pointer;
        using node_type              = impl::node<value_type>;
        using iterator               = impl::iterator<red_black_tree<value_type, Compare, Allocator>>;
        using const_iterator         = impl::const_iterator<red_black_tree<value_type, Compare, Allocator>>;
        using reverse_iterator       = impl::reverse_iterator<red_black_tree<value_type, Compare, Allocator>>;
        using const_reverse_iterator = impl::const_reverse_iterator<red_black_tree<value_type, Compare, Allocator>>;

        red_black_tree();

        template <typename T = value_type, typename = impl::disable_if_same<T, red_black_tree>>
        red_black_tree(T&& value);

        /* Value type will be smallest possible type that can store all types of the pack */
        template <typename... Args, typename = std::enable_if_t<(sizeof...(Args) > 1) && (std::is_convertible_v<Args, value_type> && ...)>>
        red_black_tree(Args&&... values);
        
        template <typename InputIt, typename = impl::enable_if_iterator<InputIt>>
        red_black_tree(InputIt first, InputIt last);

        red_black_tree(red_black_tree const& other);
        red_black_tree(red_black_tree&& other);

        ~red_black_tree();

        red_black_tree& operator=(red_black_tree const& other) &;
        red_black_tree& operator=(red_black_tree&& other) &;

        bool empty() const;
        size_type size() const noexcept;
        size_type max_size() const noexcept;

        void clear() noexcept;

        allocator_type get_allocator() const;

        #ifdef TRBT_DEBUG
        void print() const;
        #endif

        template <typename T = value_type, typename = impl::enable_if_convertible<T, value_type>>
        std::pair<iterator, bool> insert(T&& value);
        template <typename T = value_type, typename = impl::enable_if_convertible<T, value_type>>
        iterator insert(const_iterator hint, T&& value);
        template <typename InputIt, typename = impl::enable_if_iterator<InputIt>>
        void insert(InputIt first, InputIt last);

        template <typename... Args>
        std::pair<iterator, bool> emplace(Args&&... args);

        template <typename... Args>
        iterator emplace_hint(iterator, Args&&... args);
        template <typename... Args>
        iterator emplace_hint(const_iterator, Args&&... args);

        size_type erase(value_type const& value);

        bool contains(value_type const& value) const;
        size_type count(value_type const& value) const;

        void swap(red_black_tree& other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value &&
                                                  std::is_nothrow_swappable<Compare>::value);

        iterator lower_bound(value_type const& value);
        const_iterator lower_bound(value_type const& value) const;

        iterator upper_bound(value_type const& value);
        const_iterator upper_bound(value_type const& value) const;

        #ifdef TRBT_DEBUG
        void assert_properties() const;
        #endif

        iterator begin() noexcept;
        iterator end() noexcept;
        const_iterator begin() const noexcept;
        const_iterator end() const noexcept;
        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;

        reverse_iterator rbegin() noexcept;
        reverse_iterator rend() noexcept;
        const_reverse_iterator rbegin() const noexcept;
        const_reverse_iterator rend() const noexcept;
        const_reverse_iterator crbegin() const noexcept;
        const_reverse_iterator crend() const noexcept;

        template <typename Val_, typename Comp_, typename Alloc_>
        friend bool operator==(red_black_tree<Val_, Comp_, Alloc_> const& left, red_black_tree<Val_, Comp_, Alloc_> const& right);
        template <typename Val_, typename Comp_, typename Alloc_>
        friend bool operator!=(red_black_tree<Val_, Comp_, Alloc_> const& left, red_black_tree<Val_, Comp_, Alloc_> const& right);
        template <typename Val_, typename Comp_, typename Alloc_>
        friend bool operator<(red_black_tree<Val_, Comp_, Alloc_> const& left, red_black_tree<Val_, Comp_, Alloc_> const& right);
        template <typename Val_, typename Comp_, typename Alloc_>
        friend bool operator<=(red_black_tree<Val_, Comp_, Alloc_> const& left, red_black_tree<Val_, Comp_, Alloc_> const& right);
        template <typename Val_, typename Comp_, typename Alloc_>
        friend bool operator>(red_black_tree<Val_, Comp_, Alloc_> const& left, red_black_tree<Val_, Comp_, Alloc_> const& right);
        template <typename Val_, typename Comp_, typename Alloc_>
        friend bool operator>=(red_black_tree<Val_, Comp_, Alloc_> const& left, red_black_tree<Val_, Comp_, Alloc_> const& right);
        template <typename Val_, typename Comp_, typename Alloc_>
        friend void swap(red_black_tree<Val_, Comp_, Alloc_>& left, red_black_tree<Val_, Comp_, Alloc_>& right) noexcept(noexcept(left.swap(right)));

    private:
        node_type* header_;
        node_type* null_node_;
        size_type size_{};
        Alloc allocator_{};
        value_compare compare_{};

        template <typename T = value_type>
        node_type* allocate(T&& value, node_type* lc, node_type* rc, Color col, unsigned char thread);

        void init(unsigned char thread);
        void clear(node_type* current) noexcept;

        node_type* clone(node_type* pred, node_type* succ, node_type* other);

        node_type* link(node_type* node, Direction dir) const;
        node_type* min_node() const;
        node_type* max_node() const;
        static node_type* min_node(node_type* root);
        static node_type* max_node(node_type* root);
        static node_type* successor(node_type* node);
        static node_type* predecessor(node_type* node);

        static node_type* left_rotate(node_type* root, node_type* parent);
        static node_type* right_rotate(node_type* root, node_type* parent);
        static node_type* left_right_rotate(node_type* root, node_type* parent);
        static node_type* right_left_rotate(node_type* root, node_type* parent);
        static node_type* left_left_rotate(node_type* root, node_type* parent);
        static node_type* right_right_rotate(node_type* root, node_type* parent);

        void move_red_up(node_type* current, node_type* parent, node_type* grandparent, node_type* great_grandparent);
        void move_red_down(Direction dir, node_type* current, node_type*& parent, node_type* grandparent, node_type* sibling);

        node_type* unlink_node(node_type* internal, node_type* internal_parent, node_type* descendant, node_type* descendant_parent);

        template <typename T = value_type>
        std::pair<iterator, bool> insert(T&& value, node_type* current);
        template <typename T = value_type>
        iterator insert(const_iterator, T&& value, node_type* current);

        bool insert(node_type* current, node_type* new_node);
        
        template <typename... Args>
        std::pair<iterator, bool> emplace(node_type* current, Args&&... args);

        template <typename... Args>
        iterator emplace_hint(node_type* current, Args&&... args);
        
        size_type erase(value_type const& value, node_type* current);

        node_type* lower_bound(value_type const& value, node_type* current) const;
        node_type* upper_bound(value_type const& value, node_type* current) const;

        #ifdef TRBT_DEBUG
        void print(node_type* t, unsigned indentation = 0) const;
        #endif

        #ifdef TRBT_DEBUG
        int assert_properties(node_type* t) const;
        #endif
};

/* Deduction guides */
template <typename InputIt, 
          typename Compare = std::less<typename std::iterator_traits<InputIt>::value_type>,
          typename Allocator = std::allocator<typename std::iterator_traits<InputIt>::value_type>>
red_black_tree(InputIt, InputIt, Compare = Compare{}, Allocator = Allocator{})
    -> red_black_tree<typename std::iterator_traits<InputIt>::value_type, Compare, Allocator>;

template <typename InputIt, class Allocator>
red_black_tree(InputIt, InputIt, Allocator)
    -> red_black_tree<typename std::iterator_traits<InputIt>::value_type,
                      std::less<typename std::iterator_traits<InputIt>::value_type>, Allocator>;

/* Catches map overload as well */
template <typename... Args, 
          typename Compare = std::less<impl::compare_type_t<std::common_type_t<Args...>>>,
          typename Allocator = std::allocator<std::common_type_t<Args...>>>
red_black_tree(Args...)
    -> red_black_tree<std::common_type_t<Args...>, Compare, Allocator>;

/* Member functions */
TRBT_TYPE_LIST
TRBT_MEM_SPEC red_black_tree() {
    init(0x3);
}

TRBT_TYPE_LIST
template <typename T, typename>
TRBT_MEM_SPEC red_black_tree(T&& value) {
    init(0x2);
    header_->right = allocate(std::forward<T>(value), header_, header_, Color::Black, 0x3);
}

TRBT_TYPE_LIST
template <typename... Args, typename>
TRBT_MEM_SPEC red_black_tree(Args&&... values) {
    init(0x3);
    (insert(std::forward<Args>(values)), ...);
}

TRBT_TYPE_LIST
template <typename InputIt, typename>
TRBT_MEM_SPEC red_black_tree(InputIt first, InputIt last) {
    init(0x3);
    insert(first, last);
}

TRBT_TYPE_LIST
TRBT_MEM_SPEC red_black_tree(red_black_tree const& other) {
    init(other.header_->thread);
    size_ = other.size_;
    if(!other.header_->is_leaf())
        header_->right = clone(header_, header_, other.header_->right);
}

TRBT_TYPE_LIST
TRBT_MEM_SPEC red_black_tree(red_black_tree&& other) : header_{other.header_}, null_node_{other.null_node_}, size_{other.size_} {
    /* Reset other to empty state */
    other.init(0x3);
    other.size_ = 0u;
}

TRBT_TYPE_LIST
TRBT_MEM_SPEC ~red_black_tree() {
    clear();
    allocator_.deallocate(header_, 1u);
    allocator_.deallocate(null_node_, 1u);
}

TRBT_TYPE_LIST
TRBT_RED_BLACK_TREE_REF TRBT_MEM_SPEC operator=(red_black_tree const& other) & {
    auto cpy{other};
    std::swap(header_, cpy.header_);
    std::swap(size_, cpy.size_);
    return *this;
}

TRBT_TYPE_LIST
TRBT_RED_BLACK_TREE_REF TRBT_MEM_SPEC operator=(red_black_tree&& other) & {
    init(0x3);
    std::swap(header_, other.header_);
    std::swap(size_, other.size_);
    return *this;
}

TRBT_TYPE_LIST
bool TRBT_MEM_SPEC empty() const {
    return header_->thread & 0x1;
}

TRBT_TYPE_LIST
TRBT_SIZE_TYPE TRBT_MEM_SPEC size() const noexcept {
    return size_;
}

TRBT_TYPE_LIST
TRBT_SIZE_TYPE TRBT_MEM_SPEC max_size() const noexcept {
    return std::allocator_traits<Alloc>::max_size(allocator_);
}

TRBT_TYPE_LIST
void TRBT_MEM_SPEC clear() noexcept {
    if(!empty()) {
        clear(header_->right);
        
        header_->right = header_;
        header_->set_right_thread();
        size_ = 0u;
    }
}

TRBT_TYPE_LIST
TRBT_ALLOCATOR_TYPE TRBT_MEM_SPEC get_allocator() const {
    return allocator_;
}

#ifdef TRBT_DEBUG
TRBT_TYPE_LIST
void TRBT_MEM_SPEC print() const {
    if(empty())
        std::cout << "Tree is empty\n";
    else
        print(header_->right);
}
#endif

TRBT_TYPE_LIST
template <typename T, typename>
TRBT_INSERT_RETURN_PAIR TRBT_MEM_SPEC insert(T&& value) {
    if(empty()) {
        header_->right = allocate(std::forward<T>(value), header_, header_, Color::Black, 0x3);
        header_->unset_right_thread();
    
        size_++;
        
        return {iterator{this, header_->right}, true};
    }
    return insert(std::forward<T>(value), header_->right);
}

TRBT_TYPE_LIST
template <typename T, typename>
TRBT_ITER_TYPE TRBT_MEM_SPEC insert(const_iterator hint, T&& value) {
    if(empty()) {
        header_->right = allocate(std::forward<T>(value), header_, header_, Color::Black, 0x3);
        header_->unset_right_thread();
    
        size_++;
        
        return iterator{this, header_->right};
    }
    return insert(hint, std::forward<T>(value), header_->right);
}

TRBT_TYPE_LIST
template <typename InputIt, typename>
void TRBT_MEM_SPEC insert(InputIt first, InputIt last) {
    while(first != last)
        insert(*first++);
}

TRBT_TYPE_LIST
template <typename... Args>
TRBT_INSERT_RETURN_PAIR TRBT_MEM_SPEC emplace(Args&&... args) {
    if(empty()) {
        header_->right = allocate(value_type{std::forward<Args>(args)...}, header_, header_, Color::Black, 0x3);
        header_->unset_right_thread();
        
        size_++;
        return {iterator{this, header_->right}, true};
    }
    return emplace(header_->right, std::forward<Args>(args)...);
}

TRBT_TYPE_LIST
template <typename... Args>
TRBT_ITER_TYPE TRBT_MEM_SPEC emplace_hint(iterator, Args&&... args) {
    if(empty()) {
        header_->right = allocate(value_type{std::forward<Args>(args)...}, header_, header_, Color::Black, 0x3);
        header_->unset_right_thread();
        
        size_++;
        return iterator{this, header_->right};
    }
    return emplace_hint(header_->right, std::forward<Args>(args)...);
}

TRBT_TYPE_LIST
template <typename... Args>
TRBT_ITER_TYPE TRBT_MEM_SPEC emplace_hint(const_iterator, Args&&... args) {
    return emplace_hint(cend(), std::forward<Args>(args)...);
}

TRBT_TYPE_LIST
TRBT_SIZE_TYPE TRBT_MEM_SPEC erase(value_type const& value) {
    if(empty())
        return 0u;
    
    return erase(value, header_->right);
}

TRBT_TYPE_LIST
bool TRBT_MEM_SPEC contains(value_type const& value) const  {
    if(empty())
        return false;

    node_type* current = header_->right;

    while(true) {
        if(compare_(value, current->value)) {
            if(current->has_left_child())
                current = current->left;
            else
                return false;
        }
        else if(compare_(current->value, value)) {
            if(current->has_right_child())
                current = current->right;
            else
                return false;
        }
        else
            break;
    }

    return true;
} 

TRBT_TYPE_LIST
TRBT_SIZE_TYPE TRBT_MEM_SPEC count(value_type const& value) const {
    return static_cast<size_type>(contains(value));
}

TRBT_TYPE_LIST
void TRBT_MEM_SPEC swap(red_black_tree& other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value &&
                                                        std::is_nothrow_swappable<Compare>::value) {
    std::swap(header_, other.header_);
    std::swap(size_, other.size_);
}

TRBT_TYPE_LIST
TRBT_ITER_TYPE TRBT_MEM_SPEC lower_bound(value_type const& value) {
    return iterator{this, lower_bound(value, header_->right)};
}

TRBT_TYPE_LIST
TRBT_CONST_ITER_TYPE TRBT_MEM_SPEC lower_bound(value_type const& value) const {
    return const_iterator{this, lower_bound(value, header_->right)};
}

TRBT_TYPE_LIST
TRBT_ITER_TYPE TRBT_MEM_SPEC upper_bound(value_type const& value) {
    return iterator{this, upper_bound(value, header_->right)};
}

TRBT_TYPE_LIST 
TRBT_CONST_ITER_TYPE TRBT_MEM_SPEC upper_bound(value_type const& value) const {
    return const_iterator{this, upper_bound(value, header_->right)};
}

#ifdef TRBT_DEBUG
TRBT_TYPE_LIST
void TRBT_MEM_SPEC assert_properties() const {
    if(!empty()) {
        if(header_->right->color != Color::Black)
            throw color_violation_exception{"Root is red\n"};
        
        assert_properties(header_->right);
    }
}
#endif

TRBT_TYPE_LIST
TRBT_ITER_TYPE TRBT_MEM_SPEC begin() noexcept {
    return iterator{this, min_node()};
}

TRBT_TYPE_LIST
TRBT_ITER_TYPE TRBT_MEM_SPEC end() noexcept {
    return iterator{this};
}

TRBT_TYPE_LIST
TRBT_CONST_ITER_TYPE TRBT_MEM_SPEC begin() const noexcept {
    return const_iterator{this, min_node()};
}

TRBT_TYPE_LIST
TRBT_CONST_ITER_TYPE TRBT_MEM_SPEC end() const noexcept {
    return const_iterator{this};
}

TRBT_TYPE_LIST
TRBT_CONST_ITER_TYPE TRBT_MEM_SPEC cbegin() const noexcept {
    return const_iterator{this, min_node()};
}

TRBT_TYPE_LIST
TRBT_CONST_ITER_TYPE TRBT_MEM_SPEC cend() const noexcept {
    return const_iterator{this};
}
    
TRBT_TYPE_LIST
TRBT_REVERSE_ITER_TYPE TRBT_MEM_SPEC rbegin() noexcept {
    return reverse_iterator{this, max_node()};
}

TRBT_TYPE_LIST
TRBT_REVERSE_ITER_TYPE TRBT_MEM_SPEC rend() noexcept {
    return reverse_iterator{this};
}

TRBT_TYPE_LIST
TRBT_CONST_REVERSE_ITER_TYPE TRBT_MEM_SPEC rbegin() const noexcept {
    return const_reverse_iterator{this, max_node()};
}

TRBT_TYPE_LIST
TRBT_CONST_REVERSE_ITER_TYPE TRBT_MEM_SPEC rend() const noexcept {
    return const_reverse_iterator{this};
}

TRBT_TYPE_LIST
TRBT_CONST_REVERSE_ITER_TYPE TRBT_MEM_SPEC crbegin() const noexcept {
    return const_reverse_iterator{this, max_node()};
}

TRBT_TYPE_LIST
TRBT_CONST_REVERSE_ITER_TYPE TRBT_MEM_SPEC crend() const noexcept {
    return const_reverse_iterator{this};
}

template <typename Val_, typename Comp_, typename Alloc_>
bool operator==(red_black_tree<Val_, Comp_, Alloc_> const& left, red_black_tree<Val_, Comp_, Alloc_> const& right) {
    if(left.size() != right.size())
        return false;

    auto left_it = std::cbegin(left);
    auto right_it = std::cbegin(right);
    while(left_it != std::cend(left) && right_it != std::cend(right))
        if(*left_it++ != *right_it++)
            return false;

    return true;
}

template <typename Val_, typename Comp_, typename Alloc_>
bool operator!=(red_black_tree<Val_, Comp_, Alloc_> const& left, red_black_tree<Val_, Comp_, Alloc_> const& right) {
    return !(left == right);
}

template <typename Val_, typename Comp_, typename Alloc_>
bool operator<(red_black_tree<Val_, Comp_, Alloc_> const& left, red_black_tree<Val_, Comp_, Alloc_> const& right) {
    Comp_ compare{};
    auto left_it = std::cbegin(left);
    auto right_it = std::cbegin(right);
    while(left_it != std::cend(left) && right_it != std::cend(right)) {
        if(compare(*left_it, *right_it))
            return true;
        else if(compare(*right_it++, *left_it++))
            return false;
    }

    return left.size() < right.size();
}

template <typename Val_, typename Comp_, typename Alloc_>
bool operator>(red_black_tree<Val_, Comp_, Alloc_> const& left, red_black_tree<Val_, Comp_, Alloc_> const& right) {
    Comp_ compare{};
    auto left_it = std::cbegin(left);
    auto right_it = std::cbegin(right);
    while(left_it != std::cend(left) && right_it != std::cend(right)) {
        if(compare(*left_it, *right_it))
            return false;
        else if(compare(*right_it++, *left_it++))
            return true;
    }

    return left.size() > right.size();
}

template <typename Val_, typename Comp_, typename Alloc_>
bool operator<=(red_black_tree<Val_, Comp_, Alloc_> const& left, red_black_tree<Val_, Comp_, Alloc_> const& right) {
    return !(left > right);
}

template <typename Val_, typename Comp_, typename Alloc_>
bool operator>=(red_black_tree<Val_, Comp_, Alloc_> const& left, red_black_tree<Val_, Comp_, Alloc_> const& right) {
    return !(left < right);
}

template <typename Val_, typename Comp_, typename Alloc_>
void swap(red_black_tree<Val_, Comp_, Alloc_>& left, red_black_tree<Val_, Comp_, Alloc_>& right) noexcept(noexcept(left.swap(right))) {
    left.swap(right);
}

#ifdef TRBT_DEBUG
TRBT_TYPE_LIST
void TRBT_MEM_SPEC print(node_type* t, unsigned indentation) const {
    std::cout << std::setw(indentation) << "" << t->value << "\n";
    indentation += 2;

    if(t->has_left_child())
        print(t->left, indentation);
    if(t->has_right_child())
        print(t->right, indentation);
}
#endif

TRBT_TYPE_LIST
template <typename T>
TRBT_NODE_PTR TRBT_MEM_SPEC allocate(T&& value, node_type* lc, node_type* rc, Color col, unsigned char thread) {
    node_type* node = allocator_.allocate(1u);
    node = new (node) node_type{std::forward<T>(value), lc, rc, col, thread};

    return node;
}

TRBT_TYPE_LIST
void TRBT_MEM_SPEC init(unsigned char thread) {
    header_ = allocate(value_type{}, nullptr, nullptr, Color::Black, thread);
    header_->left = header_;

    if(header_->is_leaf())
        header_->right = header_;
    else
        size_++;
    
    null_node_ = allocate(value_type{}, nullptr, nullptr, Color::Black, 0x3);
    null_node_->left = null_node_->right = null_node_;
}

TRBT_TYPE_LIST
void TRBT_MEM_SPEC clear(node_type* current) noexcept {
    if(current->has_left_child())
        clear(current->left);
    if(current->has_right_child())
        clear(current->right);
    
    allocator_.deallocate(current, 1u);
}

TRBT_TYPE_LIST
TRBT_NODE_PTR TRBT_MEM_SPEC clone(node_type* pred, node_type* succ, node_type* other) {
    node_type* node = allocate(other->value, pred, succ, other->color, other->thread);

    if(other->has_left_child())
        node->left = clone(pred, node, other->left);
    if(other->has_right_child())
        node->right = clone(node, succ, other->right);

    return node;
}

TRBT_TYPE_LIST
TRBT_NODE_PTR TRBT_MEM_SPEC link(node_type* node, Direction dir) const {
    if(dir == Direction::Right)
        return node->has_right_child() ? node->right : null_node_;
    
    return node->has_left_child() ? node->left : null_node_;
}

TRBT_TYPE_LIST
TRBT_NODE_PTR TRBT_MEM_SPEC min_node() const {
    return min_node(header_->right);
}

TRBT_TYPE_LIST
TRBT_NODE_PTR TRBT_MEM_SPEC max_node() const {
    return max_node(header_->right);
}

TRBT_TYPE_LIST
TRBT_NODE_PTR TRBT_MEM_SPEC min_node(node_type* root) {
    while(root->has_left_child())
        root = root->left;

    return root;
}

TRBT_TYPE_LIST
TRBT_NODE_PTR TRBT_MEM_SPEC max_node(node_type* root) {
    while(root->has_right_child())
        root = root->right;

    return root;
}

TRBT_TYPE_LIST
TRBT_NODE_PTR TRBT_MEM_SPEC successor(node_type* node) {
    return node->has_right_child() ? min_node(node->right) : node->right;
}

TRBT_TYPE_LIST
TRBT_NODE_PTR TRBT_MEM_SPEC predecessor(node_type* node) {
    return node->has_left_child() ? max_node(node->left) : node->left;
}

TRBT_TYPE_LIST
TRBT_NODE_PTR TRBT_MEM_SPEC left_rotate(node_type* root, node_type* parent) {
    node_type* new_root = root->right;

    if(new_root->has_left_child())
        root->right = new_root->left;
    else {
        /* root->right should be thread */
        root->set_right_thread();
        /* new_root->left no longer thread */
        new_root->unset_left_thread();
    }

    new_root->left = root;

    if(parent->left == root)
        parent->left = new_root;
    else
        parent->right = new_root;

    root->color = Color::Red;
    new_root->color = Color::Black;

    return new_root;
}

TRBT_TYPE_LIST
TRBT_NODE_PTR TRBT_MEM_SPEC right_rotate(node_type* root, node_type* parent) {
    node_type* new_root = root->left;

    if(new_root->has_right_child())
        root->left = new_root->right;
    else {
        /* root->left should be thread */
        root->set_left_thread();
        /* new_root->right no longer thread */
        new_root->unset_right_thread();
    }

    new_root->right = root;
    
    if(parent->left == root)
        parent->left = new_root;
    else
        parent->right = new_root;

    root->color = Color::Red;
    new_root->color = Color::Black;

    return new_root;
}

TRBT_TYPE_LIST
TRBT_NODE_PTR TRBT_MEM_SPEC left_right_rotate(node_type* root, node_type* parent) {
    left_rotate(root->left, root);
    return right_rotate(root, parent);
}

TRBT_TYPE_LIST
TRBT_NODE_PTR TRBT_MEM_SPEC right_left_rotate(node_type* root, node_type* parent) {
    right_rotate(root->right, root);
    return left_rotate(root, parent);
}

TRBT_TYPE_LIST
TRBT_NODE_PTR TRBT_MEM_SPEC left_left_rotate(node_type* root, node_type* parent) {
    right_rotate(root->left, root);
    return right_rotate(root, parent);
}

TRBT_TYPE_LIST
TRBT_NODE_PTR TRBT_MEM_SPEC right_right_rotate(node_type* root, node_type* parent) {
    left_rotate(root->right, root);
    return left_rotate(root, parent);
}

TRBT_TYPE_LIST
void TRBT_MEM_SPEC move_red_up(node_type* current, node_type* parent, node_type* grandparent, node_type* great_grandparent) {
    current->color = Color::Red;

    if(current->has_left_child() && current->has_right_child())
        current->left->color = current->right->color = Color::Black;

    /* parent and grandparent are red, have to rotate */
    if(parent->color == Color::Red) {
        grandparent->color = Color::Red;

        bool current_is_left_child = compare_(current->value, parent->value);
        /* Nodes are on a line, double rotation */
        if(current_is_left_child != compare_(current->value, grandparent->value)) {
            if(current_is_left_child)
                right_left_rotate(grandparent, great_grandparent);
            else
                left_right_rotate(grandparent, great_grandparent);

            current->color = Color::Black;
        }
        /* Single rotation */
        else {
            if(compare_(parent->value, grandparent->value))
                right_rotate(grandparent, great_grandparent);
            else
                left_rotate(grandparent, great_grandparent);

            parent->color = Color::Black;
        }
    }
}

TRBT_TYPE_LIST
void TRBT_MEM_SPEC move_red_down(Direction dir, node_type* current, node_type*& parent, node_type* grandparent, node_type* sibling) {
    /* Node in opposite direciton is red, current and link(current, dir) are black. 
     * rotate red node into the path */
    if(link(current, !dir)->color == Color::Red) {
        if(dir == Direction::Left)
            parent = left_rotate(current, parent);
        else
            parent = right_rotate(current, parent);
    }
    else {
        /* Both of sibling's children are black, safe to recolor */
        if(link(sibling, dir)->color == Color::Black && link(sibling, !dir)->color == Color::Black) {
            parent->color = Color::Black;
            sibling->color = current->color = Color::Red;
        }
        /* Sibling has red child(ren), rotate */
        else {
            Direction last           = static_cast<Direction>(parent->right == current);
            Direction second_to_last = static_cast<Direction>(grandparent->right == parent);

            /* Triangle case */
            if(link(sibling, last)->color == Color::Red) {
                if(last == Direction::Right)
                    left_right_rotate(parent, grandparent);
                else
                    right_left_rotate(parent, grandparent);
            }
            /* Nodes are on a line */
            else {
                if(last == Direction::Left)
                    left_rotate(parent, grandparent);
                else
                    right_rotate(parent, grandparent);
            }

            node_type* new_parent = link(grandparent, second_to_last);

            /* Enforce correct coloring */
            current->color = new_parent->color = Color::Red;
            new_parent->left->color = new_parent->right->color = Color::Black;
        }
    }

    null_node_->color = Color::Black;
}

TRBT_TYPE_LIST
TRBT_NODE_PTR TRBT_MEM_SPEC unlink_node(node_type* internal, node_type* internal_parent, node_type* descendant, node_type* descendant_parent){
    /* Unlinking a leaf */
    if(internal->is_leaf()) {
        if(internal_parent->left == internal) {
            internal_parent->left = internal->left;
            internal_parent->set_left_thread();
        }
        else {
            internal_parent->right = internal->right;
            internal_parent->set_right_thread();
        }
    }
    /* 2 children, replace internal with descendant and unlink the former */
    else if(internal->has_left_child() && internal->has_right_child()) {
        if(descendant->has_left_child()) {
            if(descendant_parent->left == descendant)
                descendant_parent->left = descendant->left;
            else
                descendant_parent->right = descendant->left;
        }
        /* descendant is left child of internal, make descendant_parent->left point to
         * descendant's left and make it a thread. */
        else if(descendant_parent->left == descendant) {
            descendant_parent->left = descendant->left;
            descendant_parent->set_left_thread();
        }
        /* descendant is right child of its parent, descendant->right should still point to 
         * descendant but should be marked as a thread */
        else
            descendant_parent->set_right_thread();

        /* Make sure descendant matches internal */
        descendant->thread = internal->thread;
        descendant->color  = internal->color;
        descendant->left   = internal->left;
        descendant->right  = internal->right;

        successor(internal)->left = descendant;

        /* Replace */
        if(internal_parent->left == internal)
            internal_parent->left = descendant;
        else
            internal_parent->right = descendant;
    }
    /* Node to unlink has exactly 1 child */
    else {
        node_type* child = internal->has_left_child() ? internal->left : internal->right;
        /* Avoid color voilations */
        child->color = internal->color;
        
        if(internal_parent->left == internal)
            internal_parent->left = child;
        else
            internal_parent->right = child;

        node_type *pred = predecessor(internal), *succ = successor(internal);

        if(internal->has_left_child())
            pred->right = succ;
        else
            succ->left = pred;
    }

    return internal;
}

TRBT_TYPE_LIST
template <typename T>
TRBT_INSERT_RETURN_PAIR TRBT_MEM_SPEC insert(T&& value, node_type* current) {
    node_type *parent = header_, *grandparent = null_node_, *great_grandparent = null_node_;

    Direction dir;

    while(true) {
        /* Both current's left and right children are red (and non-threads), move red nodes upwards */
        if(link(current, Direction::Left)->color == Color::Red && link(current, Direction::Right)->color == Color::Red)
            move_red_up(current, parent, grandparent, great_grandparent);

        great_grandparent = grandparent;
        grandparent = parent;
        parent = current;
    
        if(compare_(value, current->value)) {
            dir = Direction::Left;

            /* Insert */
            if(!current->has_left_child()) {
                current->unset_left_thread();
                current->left = allocate(std::forward<T>(value), current->left, current, Color::Red, 0x3);
                current = current->left;
                break;
            }
        }
        else if(compare_(current->value, value)) {
            dir = Direction::Right;
            
            /* Insert */
            if(!current->has_right_child()) {
                current->unset_right_thread();
                current->right = allocate(std::forward<T>(value), current, current->right, Color::Red, 0x3);
                current = current->right;
                break;
            }
        }
        /* Already in tree */
        else {
            /* Ensure root is black */
            header_->right->color = Color::Black;
            return {iterator{this, current}, false};
        }

        /* Move down */
        current = link(current, dir);
    }
    move_red_up(current, parent, grandparent, great_grandparent);

    /* Ensure root is black */
    header_->right->color = Color::Black;
    ++size_;
    
    return {iterator{this, current}, true};
}

TRBT_TYPE_LIST
template <typename T>
TRBT_ITER_TYPE TRBT_MEM_SPEC insert(const_iterator, T&& value, node_type* current) {
    node_type *parent = header_, *grandparent = null_node_, *great_grandparent = null_node_;

    Direction dir;

    while(true) {
        /* Both current's left and right children are red (and non-threads), move red nodes upwards */
        if(link(current, Direction::Left)->color == Color::Red && link(current, Direction::Right)->color == Color::Red)
            move_red_up(current, parent, grandparent, great_grandparent);

        great_grandparent = grandparent;
        grandparent = parent;
        parent = current;
    
        if(compare_(value, current->value)) {
            dir = Direction::Left;

            /* Insert */
            if(!current->has_left_child()) {
                current->unset_left_thread();
                current->left = allocate(std::forward<T>(value), current->left, current, Color::Red, 0x3);
                current = current->left;
                break;
            }
        }
        else if(compare_(current->value, value)) {
            dir = Direction::Right;
            
            /* Insert */
            if(!current->has_right_child()) {
                current->unset_right_thread();
                current->right = allocate(std::forward<T>(value), current, current->right, Color::Red, 0x3);
                current = current->right;
                break;
            }
        }
        /* Already in tree */
        else {
            /* Ensure root is black */
            header_->right->color = Color::Black;
            return iterator{this, current};
        }

        /* Move down */
        current = link(current, dir);
    }
    move_red_up(current, parent, grandparent, great_grandparent);

    /* Ensure root is black */
    header_->right->color = Color::Black;
    ++size_;
    
    return iterator{this, current};
}

TRBT_TYPE_LIST
bool TRBT_MEM_SPEC insert(node_type* current, node_type* new_node) {
    node_type *parent = header_, *grandparent = null_node_, *great_grandparent = null_node_;

    Direction dir;

    while(true) {
        /* Both current's left and right children are red (and non-threads), move red nodes upwards */
        if(link(current, Direction::Left)->color == Color::Red && link(current, Direction::Right)->color == Color::Red)
            move_red_up(current, parent, grandparent, great_grandparent);

        great_grandparent = grandparent;
        grandparent = parent;
        parent = current;
    
        if(compare_(new_node->value, current->value)) {
            dir = Direction::Left;

            /* Insert */
            if(!current->has_left_child()) {
                current->unset_left_thread();
                new_node->left = current->left;
                new_node->right = current;
                new_node->color = Color::Red;
                new_node->thread = 0x3;
                current->left = new_node;
                current = current->left;
                break;
            }
        }
        else if(compare_(current->value, new_node->value)) {
            dir = Direction::Right;
            
            /* Insert */
            if(!current->has_right_child()) {
                current->unset_right_thread();
                new_node->left = current;
                new_node->right = current->right;
                new_node->color = Color::Red;
                new_node->thread = 0x3;
                current->right = new_node;
                current = current->right;
                break;
            }
        }
        /* Already in tree */
        else {
            /* Ensure root is black */
            header_->right->color = Color::Black;
            return false;
        }

        /* Move down */
        current = link(current, dir);
    }
    move_red_up(current, parent, grandparent, great_grandparent);

    /* Ensure root is black */
    header_->right->color = Color::Black;
    ++size_;

    return true;
}

TRBT_TYPE_LIST
template <typename... Args>
TRBT_INSERT_RETURN_PAIR TRBT_MEM_SPEC emplace(node_type* current, Args&&... args) {
    node_type *parent = header_, *grandparent = null_node_, *great_grandparent = null_node_;

    Direction dir;
    node_type* new_node = allocate(value_type{std::forward<Args>(args)...}, nullptr, nullptr, Color::Red, 0x3);

    while(true) {
        /* Both current's left and right children are red (and non-threads), move red nodes upwards */
        if(link(current, Direction::Left)->color == Color::Red && link(current, Direction::Right)->color == Color::Red)
            move_red_up(current, parent, grandparent, great_grandparent);

        great_grandparent = grandparent;
        grandparent = parent;
        parent = current;
    
        if(compare_(new_node->value, current->value)) {
            dir = Direction::Left;

            /* Insert */
            if(!current->has_left_child()) {
                current->unset_left_thread();
                new_node->left = current->left;
                new_node->right = current;
                current->left = new_node;
                current = current->left;
                break;
            }
        }
        else if(compare_(current->value, new_node->value)) {
            dir = Direction::Right;
            
            /* Insert */
            if(!current->has_right_child()) {
                current->unset_right_thread();
                new_node->left = current;
                new_node->right = current->right;
                current->right = new_node;
                current = current->right;
                break;
            }
        }
        /* Already in tree */
        else {
            /* Ensure root is black */
            header_->right->color = Color::Black;
            return {iterator{this, current}, false};
        }

        /* Move down */
        current = link(current, dir);
    }
    move_red_up(current, parent, grandparent, great_grandparent);

    /* Ensure root is black */
    header_->right->color = Color::Black;
    ++size_;
    
    return {iterator{this, current}, true};
}

TRBT_TYPE_LIST
template <typename... Args>
TRBT_ITER_TYPE TRBT_MEM_SPEC emplace_hint(node_type* current, Args&&... args) {
    node_type *parent = header_, *grandparent = null_node_, *great_grandparent = null_node_;

    Direction dir;
    node_type* new_node = allocate(value_type{std::forward<Args>(args)...}, nullptr, nullptr, Color::Red, 0x3);

    while(true) {
        /* Both current's left and right children are red (and non-threads), move red nodes upwards */
        if(link(current, Direction::Left)->color == Color::Red && link(current, Direction::Right)->color == Color::Red)
            move_red_up(current, parent, grandparent, great_grandparent);

        great_grandparent = grandparent;
        grandparent = parent;
        parent = current;
    
        if(compare_(new_node->value, current->value)) {
            dir = Direction::Left;

            /* Insert */
            if(!current->has_left_child()) {
                current->unset_left_thread();
                new_node->left = current->left;
                new_node->right = current;
                current->left = new_node;
                current = current->left;
                break;
            }
        }
        else if(compare_(current->value, new_node->value)) {
            dir = Direction::Right;
            
            /* Insert */
            if(!current->has_right_child()) {
                current->unset_right_thread();
                new_node->left = current;
                new_node->right = current->right;
                current->right = new_node;
                current = current->right;
                break;
            }
        }
        /* Already in tree */
        else {
            /* Ensure root is black */
            header_->right->color = Color::Black;
            return iterator{this, current};
        }

        /* Move down */
        current = link(current, dir);
    }
    move_red_up(current, parent, grandparent, great_grandparent);

    /* Ensure root is black */
    header_->right->color = Color::Black;
    ++size_;
    
    return iterator{this, current};
}

TRBT_TYPE_LIST
TRBT_SIZE_TYPE TRBT_MEM_SPEC erase(value_type const& value, node_type* current) {
    node_type *parent = header_, *grandparent = null_node_, *sibling = null_node_, *found = nullptr, *found_parent = nullptr;
    
    Direction dir;
    
    while(true) {
        dir = static_cast<Direction>(value > current->value);
        
        /* Ensure node to remove is red */
        if(current->color == Color::Black && link(current, dir)->color == Color::Black) {
            move_red_down(dir, current, parent, grandparent, sibling);

            /* If found is either the current or parent, rotations during recoloring may introduce
             * a node between found_parent and found. If this happens, move found_parent down one step */
            if(found_parent && found_parent->right != found && found_parent->left != found)
                found_parent = link(found_parent, static_cast<Direction>(found->value > found_parent->value));
        }

        /* Correct node found, store and keep moving down */
        if(!compare_(current->value, value) && !compare_(current->value, value)) {
            found = current;
            found_parent = parent;
        }
        
        /* current's link in direction dir is a thread*/
        if(link(current, dir) == null_node_)
            break;

        /* Move down */
        grandparent = parent;
        parent = current;
        sibling = link(current, !dir);
        current = link(current, dir);
    } 
    size_type num_deleted = 0u;
    if(found) {
        allocator_.deallocate(unlink_node(found, found_parent, current, parent), 1u);
        --size_;
        ++num_deleted;
    }

    if(!empty())
        header_->right->color = Color::Black;

    return num_deleted;
}

TRBT_TYPE_LIST
TRBT_NODE_PTR TRBT_MEM_SPEC lower_bound(value_type const& value, node_type* current) const {
    while(true) {
        if(compare_(value, current->value)) {
            if(current == header_)
                break;

            current = current->left;
        }
        else if(compare_(current->value, value)) {
            node_type* succ = successor(current);
            if(succ == header_)
                break;
            else if(compare_(value, succ->value))
                return succ;

            current = current->right;
        }
        else
            return current;
    }
    return header_;
}

TRBT_TYPE_LIST
TRBT_NODE_PTR TRBT_MEM_SPEC upper_bound(value_type const& value, node_type* current) const {
    while(true) {
        if(compare_(value, current->value)) {
            if(current == header_)
                break;

            current = current->left;
        }
        else if(compare_(current->value, value)) {
            node_type* succ = successor(current);
            if(succ == header_)
                break;
            else if(compare_(value, succ->value))
                return succ;

            current = current->right;
        }
        else
            return successor(current);
    }
    return header_;
}


            
#ifdef TRBT_DEBUG
TRBT_TYPE_LIST
int TRBT_MEM_SPEC assert_properties(node_type* t) const {
    node_type *lh = link(t, Direction::Left), *rh = link(t, Direction::Right);
    
    if(t->color == Color::Red) {
        if(lh->color == Color::Red || rh->color == Color::Red)
            throw color_violation_exception{"Node " + std::to_string(t->value) + " is red and has red children\n"};
    }
    if((lh != null_node_ && !compare_(lh->value, t->value)) || (rh != null_node_ && (compare_(rh->value, t->value) || (compare_(rh->value, t->value) && compare_(t->value, rh->value)))))
        throw bst_property_violation_exception{"BST property violated by node " + std::to_string(t->value) + " and its childen"};

    int height_contribution = t->color == Color::Black ? 1 : 0;

    if(t->has_left_child() && t->has_right_child()) {
        int left_height = assert_properties(t->left);
        int right_height = assert_properties(t->right);

        if(left_height != right_height)
            throw height_violation_exception{"Node " + std::to_string(t->value) + ":\nleft height: " + std::to_string(left_height) + "\nright height: " + std::to_string(right_height) + "\n"};

        return left_height + height_contribution;
    }
    else if(!t->is_leaf()) {
        if(!t->has_left_child()) {
            if(assert_properties(t->right) != -1)
                throw height_violation_exception{"Node " + std::to_string(t->value) + "'s right subtree has a black height larger than the left\n"};
        }
        else if(!t->has_right_child()) {
            if(assert_properties(t->left) != -1)
                throw height_violation_exception{"Node " + std::to_string(t->value) + "'s left subtree has a black height larger then the right\n"};
        }
    }
    return height_contribution - 1;
}
#endif

/* Map version */
template <typename Pair, typename Compare, typename Allocator>
class red_black_tree<Pair, Compare, Allocator, impl::enable_if_pair<Pair>> {
    static_assert(impl::is_comparable_v<impl::compare_type_t<Pair>, Compare>, "Key type is not comparable");

    template <typename, typename, bool>
    friend class impl::trbt_iterator_base;

    using Key       = typename Pair::first_type;
    using Mapped    = typename Pair::second_type;

    using Alloc     = typename std::allocator_traits<Allocator>::template rebind_alloc<impl::node<std::pair<Key const, Mapped>>>;
    using Color     = impl::Color;
    using Direction = impl::Direction;


    public:
        using key_type               = Key;
        using mapped_type            = Mapped;
        using value_type             = std::pair<Key const, Mapped>;
        using size_type              = std::size_t;
        using difference_type        = std::ptrdiff_t;
        using key_compare            = Compare;
        using allocator_type         = Allocator;
        using reference              = value_type&;
        using const_reference        = value_type const&;
        using pointer                = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer          = typename std::allocator_traits<Allocator>::const_pointer;
        using iterator               = impl::iterator<red_black_tree<std::pair<Key, Mapped>, Compare, Allocator>>;
        using const_iterator         = impl::const_iterator<red_black_tree<std::pair<Key, Mapped>, Compare, Allocator>>;
        using reverse_iterator       = impl::reverse_iterator<red_black_tree<std::pair<Key, Mapped>, Compare, Allocator>>;
        using const_reverse_iterator = impl::const_reverse_iterator<red_black_tree<std::pair<Key, Mapped>, Compare, Allocator>>;
        using node_type              = impl::node<std::pair<Key const, Mapped>>;
    
        red_black_tree();

        template <typename T = value_type, typename = impl::disable_if_same<T, red_black_tree>>
        red_black_tree(T&& value);

        template <typename InputIt, typename = impl::enable_if_iterator<InputIt>>
        red_black_tree(InputIt first, InputIt last);

        /* Value type will be smallest possible type that can store all types of the pack */
        template <typename... Args, typename = std::enable_if_t<(sizeof...(Args) > 1) && (std::is_convertible_v<Args, value_type> && ...)>>
        red_black_tree(Args&&... values);

        red_black_tree(red_black_tree const& other);
        red_black_tree(red_black_tree&& other);

        ~red_black_tree();

        red_black_tree& operator=(red_black_tree const& other) &;
        red_black_tree& operator=(red_black_tree&& other) &;

        bool empty() const;
        size_type size() const noexcept;
        size_type max_size() const noexcept;

        void clear() noexcept;

        allocator_type get_allocator() const;

        #ifdef TRBT_DEBUG
        void print() const;
        #endif

        template <typename T = value_type, typename = impl::enable_if_convertible<T, value_type>>
        std::pair<iterator, bool> insert(T&& value);
        template <typename T = value_type, typename = impl::enable_if_convertible<T, value_type>>
        iterator insert(const_iterator hint, T&& value);
        template <typename InputIt, typename = impl::enable_if_iterator<InputIt>>
        void insert(InputIt first, InputIt last);

        template <typename... Args>
        std::pair<iterator, bool> emplace(Args&&... args);

        template <typename... Args>
        iterator emplace_hint(iterator, Args&&... args);
        template <typename... Args>
        iterator emplace_hint(const_iterator, Args&&... args);

        size_type erase(value_type const& value);

        bool contains(value_type const& value) const;
        size_type count(value_type const& value) const;

        void swap(red_black_tree& other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value &&
                                                  std::is_nothrow_swappable<Compare>::value);

        iterator lower_bound(value_type const& value);
        const_iterator lower_bound(value_type const& value) const;

        iterator upper_bound(value_type const& value);
        const_iterator upper_bound(value_type const& value) const;

        iterator begin() noexcept;
        iterator end() noexcept;
        const_iterator begin() const noexcept;
        const_iterator end() const noexcept;
        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;

        reverse_iterator rbegin() noexcept;
        reverse_iterator rend() noexcept;
        const_reverse_iterator rbegin() const noexcept;
        const_reverse_iterator rend() const noexcept;
        const_reverse_iterator crbegin() const noexcept;
        const_reverse_iterator crend() const noexcept;

        template <typename Key_, typename Mapped_, typename Comp_, typename Alloc_>
        friend bool operator==(red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& left, 
                               red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& right);

        template <typename Key_, typename Mapped_, typename Comp_, typename Alloc_>
        friend bool operator!=(red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& left, 
                               red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& right);

        template <typename Key_, typename Mapped_, typename Comp_, typename Alloc_>
        friend bool operator<(red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& left, 
                              red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& right);

        template <typename Key_, typename Mapped_, typename Comp_, typename Alloc_>
        friend bool operator<=(red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& left, 
                               red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& right);

        template <typename Key_, typename Mapped_, typename Comp_, typename Alloc_>
        friend bool operator>(red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& left, 
                              red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& right);

        template <typename Key_, typename Mapped_, typename Comp_, typename Alloc_>
        friend bool operator>=(red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& left, 
                               red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& right);

        template <typename Key_, typename Mapped_, typename Comp_, typename Alloc_>
        friend void swap(red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_>& left, 
                         red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_>& right) noexcept(noexcept(left.swap(right)));

    private:
        node_type* header_;
        node_type* null_node_;
        size_type size_{};
        Alloc allocator_{};
        Compare compare_{};

        template <typename T = value_type>
        node_type* allocate(T&& value, node_type* lc, node_type* rc, Color col, unsigned char thread);

        void init(unsigned char thread);
        void clear(node_type* current) noexcept;

        node_type* clone(node_type* pred, node_type* succ, node_type* other);

        node_type* link(node_type* node, Direction dir) const;
        node_type* min_node() const;
        node_type* max_node() const;
        static node_type* min_node(node_type* root);
        static node_type* max_node(node_type* root);
        static node_type* successor(node_type* node);
        static node_type* predecessor(node_type* node);

        static node_type* left_rotate(node_type* root, node_type* parent);
        static node_type* right_rotate(node_type* root, node_type* parent);
        static node_type* left_right_rotate(node_type* root, node_type* parent);
        static node_type* right_left_rotate(node_type* root, node_type* parent);
        static node_type* left_left_rotate(node_type* root, node_type* parent);
        static node_type* right_right_rotate(node_type* root, node_type* parent);
        
        void move_red_up(node_type* current, node_type* parent, node_type* grandparent, node_type* great_grandparent);
        void move_red_down(Direction dir, node_type* current, node_type*& parent, node_type* grandparent, node_type* sibling);

        node_type* unlink_node(node_type* internal, node_type* internal_parent, node_type* descendant, node_type* descendant_parent);

        template <typename T = value_type>
        std::pair<iterator, bool> insert(T&& value, node_type* current);
        template <typename T = value_type>
        iterator insert(const_iterator, T&& value, node_type* current);

        bool insert(node_type* current, node_type* new_node);
        
        template <typename... Args>
        std::pair<iterator, bool> emplace(node_type* current, Args&&... args);

        template <typename... Args>
        iterator emplace_hint(node_type* current, Args&&... args);

        size_type erase(value_type const& value, node_type* current);

        node_type* lower_bound(value_type const& value, node_type* current) const;
        node_type* upper_bound(value_type const& value, node_type* current) const;

        #ifdef TRBT_DEBUG
        void print(node_type* t, unsigned indentation = 0) const;
        #endif
};

/* Map deduction guides */
template <typename Key, 
          typename Mapped, 
          typename Compare = std::less<Key>, 
          typename Allocator = std::allocator<std::pair<Key, Mapped>>>
red_black_tree(std::pair<Key, Mapped>)
    -> red_black_tree<std::pair<Key const, Mapped>, Compare, Allocator>;

/* Map members */
TRBT_MAP_TYPE_LIST
TRBT_MAP_MEM_SPEC red_black_tree() {
    init(0x3);
}

TRBT_MAP_TYPE_LIST
template <typename T, typename>
TRBT_MAP_MEM_SPEC red_black_tree(T&& value) {
    init(0x2);
    header_->right = allocate(std::forward<T>(value), header_, header_, Color::Black, 0x3);
}

TRBT_MAP_TYPE_LIST
template <typename... Args, typename>
TRBT_MAP_MEM_SPEC red_black_tree(Args&&... values) {
    init(0x3);
    (insert(std::forward<Args>(values)), ...);
}

TRBT_MAP_TYPE_LIST
template <typename InputIt, typename>
TRBT_MAP_MEM_SPEC red_black_tree(InputIt first, InputIt last) {
    init(0x3);
    insert(first, last);
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_MEM_SPEC red_black_tree(red_black_tree const& other) {
    init(other.header_->thread);
    size_ = other.size_;
    if(!other.header_->is_leaf())
        header_->right = clone(header_, header_, other.header_->right);
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_MEM_SPEC red_black_tree(red_black_tree&& other) : header_{other.header_}, null_node_{other.null_node_}, size_{other.size_} {
    other.init(0x3);
    other.size_ = 0u;
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_MEM_SPEC ~red_black_tree() {
    clear();
    allocator_.deallocate(header_, 1u);
    allocator_.deallocate(null_node_, 1u);
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_RED_BLACK_TREE_REF TRBT_MAP_MEM_SPEC operator=(red_black_tree const& other) & {
    auto cpy{other};
    std::swap(header_, cpy.header_);
    std::swap(size_, cpy.size_);
    return *this;
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_RED_BLACK_TREE_REF TRBT_MAP_MEM_SPEC operator=(red_black_tree&& other) & {
    init(0x3);
    std::swap(header_, other.header_);
    std::swap(size_, other.size_);
    return *this;
}

TRBT_MAP_TYPE_LIST
bool TRBT_MAP_MEM_SPEC empty() const {
    return header_->thread & 0x1;
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_SIZE_TYPE TRBT_MAP_MEM_SPEC size() const noexcept {
    return size_;
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_SIZE_TYPE TRBT_MAP_MEM_SPEC max_size() const noexcept {
    return std::allocator_traits<Alloc>::max_size(allocator_);
}

TRBT_MAP_TYPE_LIST
void TRBT_MAP_MEM_SPEC clear() noexcept {
    if(!empty()) {
        clear(header_->right);
        
        header_->right = header_;
        header_->set_right_thread();
        size_ = 0u;
    }
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_ALLOCATOR_TYPE TRBT_MAP_MEM_SPEC get_allocator() const {
    return allocator_;
}

#ifdef TRBT_DEBUG
TRBT_MAP_TYPE_LIST
void TRBT_MAP_MEM_SPEC print() const {
    if(empty())
        std::cout << "Tree is empty\n";
    else
        print(header_->right);
}
#endif

TRBT_MAP_TYPE_LIST
template <typename T, typename>
TRBT_MAP_INSERT_RETURN_PAIR TRBT_MAP_MEM_SPEC insert(T&& value) {
    if(empty()) {
        header_->right = allocate(std::forward<T>(value), header_, header_, Color::Black, 0x3);
        header_->unset_right_thread();
    
        size_++;
        
        return {iterator{this, header_->right}, true};
    }
    return insert(std::forward<T>(value), header_->right);
}

TRBT_MAP_TYPE_LIST
template <typename T, typename>
TRBT_MAP_ITER_TYPE TRBT_MAP_MEM_SPEC insert(const_iterator hint, T&& value) {
    if(empty()) {
        header_->right = allocate(std::forward<T>(value), header_, header_, Color::Black, 0x3);
        header_->unset_right_thread();
    
        size_++;
        
        return iterator{this, header_->right};
    }
    return insert(hint, std::forward<T>(value), header_->right);
}

TRBT_MAP_TYPE_LIST
template <typename InputIt, typename>
void TRBT_MAP_MEM_SPEC insert(InputIt first, InputIt last) {
    while(first != last)
        insert(*first++);
}

TRBT_MAP_TYPE_LIST
template <typename... Args>
TRBT_MAP_INSERT_RETURN_PAIR TRBT_MAP_MEM_SPEC emplace(Args&&... args) {
    if(empty()) {
        header_->right = allocate(value_type{std::forward<Args>(args)...}, header_, header_, Color::Black, 0x3);
        header_->unset_right_thread();
        
        size_++;
        return {iterator{this, header_->right}, true};
    }
    return emplace(header_->right, std::forward<Args>(args)...);
}

TRBT_MAP_TYPE_LIST
template <typename... Args>
TRBT_MAP_ITER_TYPE TRBT_MAP_MEM_SPEC emplace_hint(iterator, Args&&... args) {
    if(empty()) {
        header_->right = allocate(value_type{std::forward<Args>(args)...}, header_, header_, Color::Black, 0x3);
        header_->unset_right_thread();
        
        size_++;
        return iterator{this, header_->right};
    }
    return emplace_hint(header_->right, std::forward<Args>(args)...);
}

TRBT_MAP_TYPE_LIST
template <typename... Args>
TRBT_MAP_ITER_TYPE TRBT_MAP_MEM_SPEC emplace_hint(const_iterator, Args&&... args) {
    return emplace_hint(cend(), std::forward<Args>(args)...);
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_SIZE_TYPE TRBT_MAP_MEM_SPEC erase(value_type const& value) {
    if(empty())
        return 0u;
    
    return erase(value, header_->right);
}

TRBT_MAP_TYPE_LIST
bool TRBT_MAP_MEM_SPEC contains(value_type const& value) const  {
    if(empty())
        return false;

    node_type* current = header_->right;

    while(true) {
        if(compare_(value.first, current->value.first)) {
            if(current->has_left_child())
                current = current->left;
            else
                return false;
        }
        else if(compare_(current->value.fist, value.first)) {
            if(current->has_right_child())
                current = current->right;
            else
                return false;
        }
        else
            break;
    }

    return true;
} 

TRBT_MAP_TYPE_LIST
TRBT_MAP_SIZE_TYPE TRBT_MAP_MEM_SPEC count(value_type const& value) const {
    return static_cast<size_type>(contains(value));
}

TRBT_MAP_TYPE_LIST
void TRBT_MAP_MEM_SPEC swap(red_black_tree& other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value &&
                                                        std::is_nothrow_swappable<Compare>::value) {
    std::swap(header_, other.header_);
    std::swap(size_, other.size_);
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_ITER_TYPE TRBT_MAP_MEM_SPEC lower_bound(value_type const& value) {
    return iterator{this, lower_bound(value, header_->right)};
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_CONST_ITER_TYPE TRBT_MAP_MEM_SPEC lower_bound(value_type const& value) const {
    return const_iterator{this, lower_bound(value, header_->right)};
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_ITER_TYPE TRBT_MAP_MEM_SPEC upper_bound(value_type const& value) {
    return iterator{this, upper_bound(value, header_->right)};
}

TRBT_MAP_TYPE_LIST 
TRBT_MAP_CONST_ITER_TYPE TRBT_MAP_MEM_SPEC upper_bound(value_type const& value) const {
    return const_iterator{this, upper_bound(value, header_->right)};
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_ITER_TYPE TRBT_MAP_MEM_SPEC begin() noexcept {
    return iterator{this, min_node()};
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_ITER_TYPE TRBT_MAP_MEM_SPEC end() noexcept {
    return iterator{this};
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_CONST_ITER_TYPE TRBT_MAP_MEM_SPEC begin() const noexcept {
    return const_iterator{this, min_node()};
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_CONST_ITER_TYPE TRBT_MAP_MEM_SPEC end() const noexcept {
    return const_iterator{this};
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_CONST_ITER_TYPE TRBT_MAP_MEM_SPEC cbegin() const noexcept {
    return const_iterator{this, min_node()};
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_CONST_ITER_TYPE TRBT_MAP_MEM_SPEC cend() const noexcept {
    return const_iterator{this};
}
    
TRBT_MAP_TYPE_LIST
TRBT_MAP_REVERSE_ITER_TYPE TRBT_MAP_MEM_SPEC rbegin() noexcept {
    return reverse_iterator{this, max_node()};
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_REVERSE_ITER_TYPE TRBT_MAP_MEM_SPEC rend() noexcept {
    return reverse_iterator{this};
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_CONST_REVERSE_ITER_TYPE TRBT_MAP_MEM_SPEC rbegin() const noexcept {
    return const_reverse_iterator{this, max_node()};
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_CONST_REVERSE_ITER_TYPE TRBT_MAP_MEM_SPEC rend() const noexcept {
    return const_reverse_iterator{this};
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_CONST_REVERSE_ITER_TYPE TRBT_MAP_MEM_SPEC crbegin() const noexcept {
    return const_reverse_iterator{this, max_node()};
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_CONST_REVERSE_ITER_TYPE TRBT_MAP_MEM_SPEC crend() const noexcept {
    return const_reverse_iterator{this};
}

template <typename Key_, typename Mapped_, typename Comp_, typename Alloc_>
bool operator==(red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& left, 
                red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& right) 
{
    if(left.size() != right.size())
        return false;

    auto left_it = std::cbegin(left);
    auto right_it = std::cbegin(right);
    while(left_it != std::cend(left) && right_it != std::cend(right))
        if((*left_it).first != (*right_it).first || (*left_it++).second != (*right_it++).second)
            return false;

    return true;
}

template <typename Key_, typename Mapped_, typename Comp_, typename Alloc_>
bool operator!=(red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& left, 
                red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& right) 
{
    return !(left == right);
}

template <typename Key_, typename Mapped_, typename Comp_, typename Alloc_>
bool operator<(red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& left, 
               red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& right) 
{
    Comp_ compare{};
    auto left_it = std::cbegin(left);
    auto right_it = std::cbegin(right);
    while(left_it != std::cend(left) && right_it != std::cend(right)) {
        if(compare((*left_it).first, (*right_it).first))
            return true;
        else if(compare((*right_it++).first, (*left_it++).first))
            return false;
    }

    return left.size() < right.size();
}

template <typename Key_, typename Mapped_, typename Comp_, typename Alloc_>
bool operator>(red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& left, 
               red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& right) 
{
    Comp_ compare{};
    auto left_it = std::cbegin(left);
    auto right_it = std::cbegin(right);
    while(left_it != std::cend(left) && right_it != std::cend(right)) {
        if(compare((*left_it).first, (*right_it).first))
            return false;
        else if(compare((*right_it++).first, (*left_it++).first))
            return true;
    }

    return left.size() > right.size();
}

template <typename Key_, typename Mapped_, typename Comp_, typename Alloc_>
bool operator<=(red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& left, 
                red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& right) 
{
    return !(left > right);
}

template <typename Key_, typename Mapped_, typename Comp_, typename Alloc_>
bool operator>=(red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& left, 
                red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_> const& right) 
{
    return !(left < right);
}

template <typename Key_, typename Mapped_, typename Comp_, typename Alloc_>
void swap(red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_>& left, 
red_black_tree<std::pair<Key_, Mapped_>, Comp_, Alloc_>& right) noexcept(noexcept(left.swap(right))) 
{
    left.swap(right);
}

#ifdef TRBT_DEBUG
TRBT_MAP_TYPE_LIST
void TRBT_MAP_MEM_SPEC print(node_type* t, unsigned indentation) const {
    std::cout << std::setw(indentation) << "" << "{" << t->value.first << ", " << t->value.second << "}\n";
    indentation += 2;

    if(t->has_left_child())
        print(t->left, indentation);
    if(t->has_right_child())
        print(t->right, indentation);
}
#endif

TRBT_MAP_TYPE_LIST
template <typename T>
TRBT_MAP_NODE_PTR TRBT_MAP_MEM_SPEC allocate(T&& value, node_type* lc, node_type* rc, Color col, unsigned char thread) {
    node_type* node = allocator_.allocate(1u);
    node = new (node) node_type{std::forward<T>(value), lc, rc, col, thread};
    
    return node;
}

TRBT_MAP_TYPE_LIST
void TRBT_MAP_MEM_SPEC init(unsigned char thread) {
    header_ = allocate(value_type{}, nullptr, nullptr, Color::Black, thread);
    header_->left = header_;

    if(header_->is_leaf())
        header_->right = header_;
    else
        size_++;
    
    null_node_ = allocate(value_type{}, nullptr, nullptr, Color::Black, 0x3);
    null_node_->left = null_node_->right = null_node_;
}

TRBT_MAP_TYPE_LIST
void TRBT_MAP_MEM_SPEC clear(node_type* current) noexcept {
    if(current->has_left_child())
        clear(current->left);
    if(current->has_right_child())
        clear(current->right);
    
    allocator_.deallocate(current, 1u);
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_NODE_PTR TRBT_MAP_MEM_SPEC clone(node_type* pred, node_type* succ, node_type* other) {
    node_type* node = allocate(other->value, pred, succ, other->color, other->thread);

    if(other->has_left_child())
        node->left = clone(pred, node, other->left);
    if(other->has_right_child())
        node->right = clone(node, succ, other->right);

    return node;
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_NODE_PTR TRBT_MAP_MEM_SPEC link(node_type* node, Direction dir) const {
    if(dir == Direction::Right)
        return node->has_right_child() ? node->right : null_node_;
    
    return node->has_left_child() ? node->left : null_node_;
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_NODE_PTR TRBT_MAP_MEM_SPEC min_node() const {
    return min_node(header_->right);
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_NODE_PTR TRBT_MAP_MEM_SPEC max_node() const {
    return max_node(header_->right);
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_NODE_PTR TRBT_MAP_MEM_SPEC min_node(node_type* root) {
    while(root->has_left_child())
        root = root->left;

    return root;
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_NODE_PTR TRBT_MAP_MEM_SPEC max_node(node_type* root) {
    while(root->has_right_child())
        root = root->right;

    return root;
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_NODE_PTR TRBT_MAP_MEM_SPEC successor(node_type* node) {
    return node->has_right_child() ? min_node(node->right) : node->right;
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_NODE_PTR TRBT_MAP_MEM_SPEC predecessor(node_type* node) {
    return node->has_left_child() ? max_node(node->left) : node->left;
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_NODE_PTR TRBT_MAP_MEM_SPEC left_rotate(node_type* root, node_type* parent) {
    node_type* new_root = root->right;

    if(new_root->has_left_child())
        root->right = new_root->left;
    else {
        /* root->right should be thread */
        root->set_right_thread();
        /* new_root->left no longer thread */
        new_root->unset_left_thread();
    }

    new_root->left = root;

    if(parent->left == root)
        parent->left = new_root;
    else
        parent->right = new_root;

    root->color = Color::Red;
    new_root->color = Color::Black;

    return new_root;
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_NODE_PTR TRBT_MAP_MEM_SPEC right_rotate(node_type* root, node_type* parent) {
    node_type* new_root = root->left;

    if(new_root->has_right_child())
        root->left = new_root->right;
    else {
        /* root->left should be thread */
        root->set_left_thread();
        /* new_root->right no longer thread */
        new_root->unset_right_thread();
    }

    new_root->right = root;
    
    if(parent->left == root)
        parent->left = new_root;
    else
        parent->right = new_root;

    root->color = Color::Red;
    new_root->color = Color::Black;

    return new_root;
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_NODE_PTR TRBT_MAP_MEM_SPEC left_right_rotate(node_type* root, node_type* parent) {
    left_rotate(root->left, root);
    return right_rotate(root, parent);
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_NODE_PTR TRBT_MAP_MEM_SPEC right_left_rotate(node_type* root, node_type* parent) {
    right_rotate(root->right, root);
    return left_rotate(root, parent);
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_NODE_PTR TRBT_MAP_MEM_SPEC left_left_rotate(node_type* root, node_type* parent) {
    right_rotate(root->left, root);
    return right_rotate(root, parent);
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_NODE_PTR TRBT_MAP_MEM_SPEC right_right_rotate(node_type* root, node_type* parent) {
    left_rotate(root->right, root);
    return left_rotate(root, parent);
}

TRBT_MAP_TYPE_LIST
void TRBT_MAP_MEM_SPEC move_red_up(node_type* current, node_type* parent, node_type* grandparent, node_type* great_grandparent) {
    current->color = Color::Red;

    if(current->has_left_child() && current->has_right_child())
        current->left->color = current->right->color = Color::Black;

    /* parent and grandparent are red, have to rotate */
    if(parent->color == Color::Red) {
        grandparent->color = Color::Red;

        bool current_is_left_child = compare_(current->value.first, parent->value.first);
        /* Nodes are on a line, double rotation */
        if(current_is_left_child != compare_(current->value.first, grandparent->value.first)) {
            if(current_is_left_child)
                right_left_rotate(grandparent, great_grandparent);
            else
                left_right_rotate(grandparent, great_grandparent);

            current->color = Color::Black;
        }
        /* Single rotation */
        else {
            if(compare_(parent->value.first, grandparent->value.first))
                right_rotate(grandparent, great_grandparent);
            else
                left_rotate(grandparent, great_grandparent);

            parent->color = Color::Black;
        }
    }
}

TRBT_MAP_TYPE_LIST
void TRBT_MAP_MEM_SPEC move_red_down(Direction dir, node_type* current, node_type*& parent, node_type* grandparent, node_type* sibling) {
    /* Node in opposite direciton is red, current and link(current, dir) are black. 
     * rotate red node into the path */
    if(link(current, !dir)->color == Color::Red) {
        if(dir == Direction::Left)
            parent = left_rotate(current, parent);
        else
            parent = right_rotate(current, parent);
    }
    else {
        /* Both of sibling's children are black, safe to recolor */
        if(link(sibling, dir)->color == Color::Black && link(sibling, !dir)->color == Color::Black) {
            parent->color = Color::Black;
            sibling->color = current->color = Color::Red;
        }
        /* Sibling has red child(ren), rotate */
        else {
            Direction last           = static_cast<Direction>(parent->right == current);
            Direction second_to_last = static_cast<Direction>(grandparent->right == parent);

            /* Triangle case */
            if(link(sibling, last)->color == Color::Red) {
                if(last == Direction::Right)
                    left_right_rotate(parent, grandparent);
                else
                    right_left_rotate(parent, grandparent);
            }
            /* Nodes are on a line */
            else {
                if(last == Direction::Left)
                    left_rotate(parent, grandparent);
                else
                    right_rotate(parent, grandparent);
            }

            node_type* new_parent = link(grandparent, second_to_last);

            /* Enforce correct coloring */
            current->color = new_parent->color = Color::Red;
            new_parent->left->color = new_parent->right->color = Color::Black;
        }
    }

    null_node_->color = Color::Black;
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_NODE_PTR TRBT_MAP_MEM_SPEC unlink_node(node_type* internal, node_type* internal_parent, node_type* descendant, node_type* descendant_parent){
    /* Unlinking a leaf */
    if(internal->is_leaf()) {
        if(internal_parent->left == internal) {
            internal_parent->left = internal->left;
            internal_parent->set_left_thread();
        }
        else {
            internal_parent->right = internal->right;
            internal_parent->set_right_thread();
        }
    }
    /* 2 children, replace internal with descendant and unlink the former */
    else if(internal->has_left_child() && internal->has_right_child()) {
        if(descendant->has_left_child()) {
            if(descendant_parent->left == descendant)
                descendant_parent->left = descendant->left;
            else
                descendant_parent->right = descendant->left;
        }
        /* descendant is left child of internal, make descendant_parent->left point to
         * descendant's left and make it a thread. */
        else if(descendant_parent->left == descendant) {
            descendant_parent->left = descendant->left;
            descendant_parent->set_left_thread();
        }
        /* descendant is right child of its parent, descendant->right should still point to 
         * descendant but should be marked as a thread */
        else
            descendant_parent->set_right_thread();

        /* Make sure descendant matches internal */
        descendant->thread = internal->thread;
        descendant->color  = internal->color;
        descendant->left   = internal->left;
        descendant->right  = internal->right;

        successor(internal)->left = descendant;

        /* Replace */
        if(internal_parent->left == internal)
            internal_parent->left = descendant;
        else
            internal_parent->right = descendant;
    }
    /* Node to unlink has exactly 1 child */
    else {
        node_type* child = internal->has_left_child() ? internal->left : internal->right;
        /* Avoid color voilations */
        child->color = internal->color;
        
        if(internal_parent->left == internal)
            internal_parent->left = child;
        else
            internal_parent->right = child;

        node_type *pred = predecessor(internal), *succ = successor(internal);

        if(internal->has_left_child())
            pred->right = succ;
        else
            succ->left = pred;
    }

    return internal;
}

TRBT_MAP_TYPE_LIST
template <typename T>
TRBT_MAP_INSERT_RETURN_PAIR TRBT_MAP_MEM_SPEC insert(T&& value, node_type* current) {
    node_type *parent = header_, *grandparent = null_node_, *great_grandparent = null_node_;

    Direction dir;

    while(true) {
        /* Both current's left and right children are red (and non-threads), move red nodes upwards */
        if(link(current, Direction::Left)->color == Color::Red && link(current, Direction::Right)->color == Color::Red)
            move_red_up(current, parent, grandparent, great_grandparent);

        great_grandparent = grandparent;
        grandparent = parent;
        parent = current;
    
        if(compare_(value.first, current->value.first)) {
            dir = Direction::Left;

            /* Insert */
            if(!current->has_left_child()) {
                current->unset_left_thread();
                current->left = allocate(std::forward<T>(value), current->left, current, Color::Red, 0x3);
                current = current->left;
                break;
            }
        }
        else if(compare_(current->value.first, value.first)) {
            dir = Direction::Right;
            
            /* Insert */
            if(!current->has_right_child()) {
                current->unset_right_thread();
                current->right = allocate(std::forward<T>(value), current, current->right, Color::Red, 0x3);
                current = current->right;
                break;
            }
        }
        /* Already in tree */
        else {
            /* Ensure root is black */
            header_->right->color = Color::Black;
            return {iterator{this, current}, false};
        }

        /* Move down */
        current = link(current, dir);
    }
    move_red_up(current, parent, grandparent, great_grandparent);

    /* Ensure root is black */
    header_->right->color = Color::Black;
    ++size_;
    
    return {iterator{this, current}, true};
}

TRBT_MAP_TYPE_LIST
template <typename T>
TRBT_MAP_ITER_TYPE TRBT_MAP_MEM_SPEC insert(const_iterator, T&& value, node_type* current) {
    node_type *parent = header_, *grandparent = null_node_, *great_grandparent = null_node_;

    Direction dir;

    while(true) {
        /* Both current's left and right children are red (and non-threads), move red nodes upwards */
        if(link(current, Direction::Left)->color == Color::Red && link(current, Direction::Right)->color == Color::Red)
            move_red_up(current, parent, grandparent, great_grandparent);

        great_grandparent = grandparent;
        grandparent = parent;
        parent = current;
    
        if(compare_(value.first, current->value.first)) {
            dir = Direction::Left;

            /* Insert */
            if(!current->has_left_child()) {
                current->unset_left_thread();
                current->left = allocate(std::forward<T>(value), current->left, current, Color::Red, 0x3);
                current = current->left;
                break;
            }
        }
        else if(compare_(current->value.first, value.first)) {
            dir = Direction::Right;
            
            /* Insert */
            if(!current->has_right_child()) {
                current->unset_right_thread();
                current->right = allocate(std::forward<T>(value), current, current->right, Color::Red, 0x3);
                current = current->right;
                break;
            }
        }
        /* Already in tree */
        else {
            /* Ensure root is black */
            header_->right->color = Color::Black;
            return iterator{this, current};
        }

        /* Move down */
        current = link(current, dir);
    }
    move_red_up(current, parent, grandparent, great_grandparent);

    /* Ensure root is black */
    header_->right->color = Color::Black;
    ++size_;
    
    return iterator{this, current};
}

TRBT_MAP_TYPE_LIST
bool TRBT_MAP_MEM_SPEC insert(node_type* current, node_type* new_node) {
    node_type *parent = header_, *grandparent = null_node_, *great_grandparent = null_node_;

    Direction dir;

    while(true) {
        /* Both current's left and right children are red (and non-threads), move red nodes upwards */
        if(link(current, Direction::Left)->color == Color::Red && link(current, Direction::Right)->color == Color::Red)
            move_red_up(current, parent, grandparent, great_grandparent);

        great_grandparent = grandparent;
        grandparent = parent;
        parent = current;
    
        if(compare_(new_node->value.first, current->value.first)) {
            dir = Direction::Left;

            /* Insert */
            if(!current->has_left_child()) {
                current->unset_left_thread();
                new_node->left = current->left;
                new_node->right = current;
                new_node->color = Color::Red;
                new_node->thread = 0x3;
                current->left = new_node;
                current = current->left;
                break;
            }
        }
        else if(compare_(current->value.first, new_node->value.first)) {
            dir = Direction::Right;
            
            /* Insert */
            if(!current->has_right_child()) {
                current->unset_right_thread();
                new_node->left = current;
                new_node->right = current->right;
                new_node->color = Color::Red;
                new_node->thread = 0x3;
                current->right = new_node;
                current = current->right;
                break;
            }
        }
        /* Already in tree */
        else {
            /* Ensure root is black */
            header_->right->color = Color::Black;
            return false;
        }

        /* Move down */
        current = link(current, dir);
    }
    move_red_up(current, parent, grandparent, great_grandparent);

    /* Ensure root is black */
    header_->right->color = Color::Black;
    ++size_;

    return true;
}

TRBT_MAP_TYPE_LIST
template <typename... Args>
TRBT_MAP_INSERT_RETURN_PAIR TRBT_MAP_MEM_SPEC emplace(node_type* current, Args&&... args) {
    node_type *parent = header_, *grandparent = null_node_, *great_grandparent = null_node_;

    Direction dir;
    node_type* new_node = allocate(value_type{std::forward<Args>(args)...}, nullptr, nullptr, Color::Red, 0x3);

    while(true) {
        /* Both current's left and right children are red (and non-threads), move red nodes upwards */
        if(link(current, Direction::Left)->color == Color::Red && link(current, Direction::Right)->color == Color::Red)
            move_red_up(current, parent, grandparent, great_grandparent);

        great_grandparent = grandparent;
        grandparent = parent;
        parent = current;
    
        if(compare_(new_node->value.first, current->value.first)) {
            dir = Direction::Left;

            /* Insert */
            if(!current->has_left_child()) {
                current->unset_left_thread();
                new_node->left = current->left;
                new_node->right = current;
                current->left = new_node;
                current = current->left;
                break;
            }
        }
        else if(compare_(current->value.first, new_node->value.first)) {
            dir = Direction::Right;
            
            /* Insert */
            if(!current->has_right_child()) {
                current->unset_right_thread();
                new_node->left = current;
                new_node->right = current->right;
                current->right = new_node;
                current = current->right;
                break;
            }
        }
        /* Already in tree */
        else {
            /* Ensure root is black */
            header_->right->color = Color::Black;
            return {iterator{this, current}, false};
        }

        /* Move down */
        current = link(current, dir);
    }
    move_red_up(current, parent, grandparent, great_grandparent);

    /* Ensure root is black */
    header_->right->color = Color::Black;
    ++size_;
    
    return {iterator{this, current}, true};
}

TRBT_MAP_TYPE_LIST
template <typename... Args>
TRBT_MAP_ITER_TYPE TRBT_MAP_MEM_SPEC emplace_hint(node_type* current, Args&&... args) {
    node_type *parent = header_, *grandparent = null_node_, *great_grandparent = null_node_;

    Direction dir;
    node_type* new_node = allocate(value_type{std::forward<Args>(args)...}, nullptr, nullptr, Color::Red, 0x3);

    while(true) {
        /* Both current's left and right children are red (and non-threads), move red nodes upwards */
        if(link(current, Direction::Left)->color == Color::Red && link(current, Direction::Right)->color == Color::Red)
            move_red_up(current, parent, grandparent, great_grandparent);

        great_grandparent = grandparent;
        grandparent = parent;
        parent = current;
    
        if(compare_(new_node->value.first, current->value.first)) {
            dir = Direction::Left;

            /* Insert */
            if(!current->has_left_child()) {
                current->unset_left_thread();
                new_node->left = current->left;
                new_node->right = current;
                current->left = new_node;
                current = current->left;
                break;
            }
        }
        else if(compare_(current->value.first, new_node->value.first)) {
            dir = Direction::Right;
            
            /* Insert */
            if(!current->has_right_child()) {
                current->unset_right_thread();
                new_node->left = current;
                new_node->right = current->right;
                current->right = new_node;
                current = current->right;
                break;
            }
        }
        /* Already in tree */
        else {
            /* Ensure root is black */
            header_->right->color = Color::Black;
            return iterator{this, current};
        }

        /* Move down */
        current = link(current, dir);
    }
    move_red_up(current, parent, grandparent, great_grandparent);

    /* Ensure root is black */
    header_->right->color = Color::Black;
    ++size_;
    
    return iterator{this, current};
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_SIZE_TYPE TRBT_MAP_MEM_SPEC erase(value_type const& value, node_type* current) {
    node_type *parent = header_, *grandparent = null_node_, *sibling = null_node_, *found = nullptr, *found_parent = nullptr;
    
    Direction dir;
    
    while(true) {
        dir = static_cast<Direction>(value.first > current->value.first);
        
        /* Ensure node to remove is red */
        if(current->color == Color::Black && link(current, dir)->color == Color::Black) {
            move_red_down(dir, current, parent, grandparent, sibling);

            /* If found is either the current or parent, rotations during recoloring may introduce
             * a node between found_parent and found. If this happens, move found_parent down one step */
            if(found_parent && found_parent->right != found && found_parent->left != found)
                found_parent = link(found_parent, static_cast<Direction>(found->value.first > found_parent->value.first));
        }

        /* Correct node found, store and keep moving down */
        if(!compare_(current->value.first, value.first) && !compare_(current->value.first, value.first)) {
            found = current;
            found_parent = parent;
        }
        
        /* current's link in direction dir is a thread*/
        if(link(current, dir) == null_node_)
            break;

        /* Move down */
        grandparent = parent;
        parent = current;
        sibling = link(current, !dir);
        current = link(current, dir);
    } 
    size_type num_deleted = 0u;
    if(found) {
        allocator_.deallocate(unlink_node(found, found_parent, current, parent), 1u);
        --size_;
        ++num_deleted;
    }

    if(!empty())
        header_->right->color = Color::Black;

    return num_deleted;
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_NODE_PTR TRBT_MAP_MEM_SPEC lower_bound(value_type const& value, node_type* current) const {
    while(true) {
        if(compare_(value.first, current->value.first)) {
            if(current == header_)
                break;

            current = current->left;
        }
        else if(compare_(current->value.first, value.first)) {
            node_type* succ = successor(current);
            if(succ == header_)
                break;
            else if(compare_(value.first, succ->value.first))
                return succ;

            current = current->right;
        }
        else
            return current;
    }
    return header_;
}

TRBT_MAP_TYPE_LIST
TRBT_MAP_NODE_PTR TRBT_MAP_MEM_SPEC upper_bound(value_type const& value, node_type* current) const {
    while(true) {
        if(compare_(value.first, current->value.first)) {
            if(current == header_)
                break;

            current = current->left;
        }
        else if(compare_(current->value.first, value.first)) {
            node_type* succ = successor(current);
            if(succ == header_)
                break;
            else if(compare_(value.first, succ->value.first))
                return succ;

            current = current->right;
        }
        else
            return successor(current);
    }
    return header_;
}


#ifdef TRBT_DEBUG
namespace impl {
    namespace debug {
        inline void run_test(int test_size = 2000) {
            std::cout << "Running test with size " << test_size << std::endl;
            std::random_device rd;
            std::mt19937 mt{std::random_device{}()};
            std::vector<int> vals(test_size);

            {
                std::uniform_int_distribution<> dis{0, 10 * test_size};

                for(auto i = 0; i < test_size; i++)
                    vals.push_back(dis(mt));
            }

            vals.erase(std::unique(std::begin(vals), std::end(vals)), std::end(vals));

            red_black_tree<int> rbt{};
            for(auto const& v : vals) {
                rbt.insert(v);
                rbt.assert_properties();
            }

            for(auto const& v : vals) {
                if(!rbt.contains(v))
                    throw value_retention_exception{std::to_string(v) + " not in tree\n"};
            }
        
            while(vals.size()) {
                std::uniform_int_distribution<> dis{0, static_cast<int>(vals.size()) - 1};
                auto rand = dis(mt);
                auto val = vals[rand];

                rbt.erase(val);
                rbt.assert_properties();

                vals.erase(std::remove_if(std::begin(vals), std::end(vals), [val](int v) { 
                    return val == v; 
                }), std::end(vals));

                if(std::find(std::begin(vals), std::end(vals), val) != std::end(vals))
                    std::cout << val << "\n";
                
                for(auto const& v : vals) {
                    if(!rbt.contains(v))
                        throw value_retention_exception{std::to_string(v) + " is not in tree\n"};
                }
            }
            
            std::cout << "Test completed" << std::endl;
        }
    } /* namespace debug */
} /* namespace impl */
#endif

} /* namespace trbt */

#undef TRBT_TYPE_LIST
#undef TRBT_MEM_SPEC
#undef TRBT_NODE_PTR
#undef TRBT_SIZE_TYPE
#undef TRBT_ITER_TYPE
#undef TRBT_CONST_ITER_TYPE
#undef TRBT_REVERSE_ITER_TYPE
#undef TRBT_CONST_REVERSE_ITER_TYPE
#undef TRBT_INSERT_RETURN_TYPE
#undef TRBT_RED_BLACK_TREE_REF
#undef TRBT_ALLOCATOR_TYPE

#undef TRBT_MAP_TYPE_LIST 
#undef TRBT_MAP_MEM_SPEC 
#undef TRBT_MAP_NODE_PTR
#undef TRBT_MAP_SIZE_TYPE
#undef TRBT_MAP_ITER_TYPE
#undef TRBT_MAP_CONST_ITER_TYPE
#undef TRBT_MAP_REVERSE_ITER_TYPE
#undef TRBT_MAP_CONST_REVERSE_ITER_TYPE
#undef TRBT_MAP_RED_BLACK_TREE_REF 
#undef TRBT_MAP_ALLOCATOR_TYPE 
#undef TRBT_MAP_INSERT_RETURN_PAIR 
#endif
