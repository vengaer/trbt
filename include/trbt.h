#ifndef TRBT_H
#define TRBT_H

#pragma once
#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <tuple>
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
    template <typename, typename, typename>
    class rbtree;

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
    using disable_if_same_t = std::enable_if_t<!std::is_same_v<remove_cvref_t<T>, remove_cvref_t<U>>>;

    template <typename, typename = void>
    struct satisfies_input_iterator : std::false_type { };

    template <typename T>
    struct satisfies_input_iterator<T, std::enable_if_t<
                                            std::is_base_of_v<
                                                std::input_iterator_tag, 
                                                typename std::iterator_traits<T>::iterator_category>>> 
        : std::true_type { };

    template <typename T>
    using enable_if_iterator_t = std::enable_if_t<satisfies_input_iterator<T>::value>;

    template <typename T, typename U>
    using enable_if_convertible_t = std::enable_if_t<std::is_convertible_v<remove_cvref_t<T>, remove_cvref_t<U>>>;
    
    template <typename T>
    struct type_is {
        using type = T;
    };

    struct unavailable { };

    template <typename T>
    struct compare_type : type_is<T> { };

    template <typename K, typename M>
    struct compare_type<std::pair<K, M>> : type_is<K> { };

    template <typename T>
    using compare_type_t = typename compare_type<T>::type;

    template <typename T>
    struct key_type : type_is<T> { };

    template <typename K, typename M>
    struct key_type<std::pair<K, M>> : type_is<K> { };

    template <typename T>
    using key_type_t = typename key_type<T>::type;

    template <typename>
    struct mapped_type : type_is<unavailable> { };

    template <typename K, typename M>
    struct mapped_type<std::pair<K, M>> : type_is<M> { };

    template <typename T>
    using mapped_type_t = typename mapped_type<T>::type;

    template <typename T>
    struct is_pair : std::false_type { };

    template <typename K, typename M>
    struct is_pair<std::pair<K, M>> : std::true_type { };

    template <typename K, typename M>
    struct is_pair<std::pair<K, M> const> : std::true_type { };

    template <typename K, typename M>
    struct is_pair<std::pair<K, M> volatile> : std::true_type { };

    template <typename K, typename M>
    struct is_pair<std::pair<K, M> const volatile> : std::true_type { };

    template <typename T>
    inline bool constexpr is_pair_v = is_pair<T>::value;

    template <typename>
    struct is_map : std::false_type { };

    template <template <typename, typename, typename> typename Tree,
              typename Key,
              typename Mapped,
              typename Compare,
              typename Alloc>
    struct is_map<Tree<std::pair<Key, Mapped>, Compare, Alloc>> : std::true_type { };

    template <typename T>
    inline bool constexpr is_map_v = is_map<T>::value;

    template <typename T>
    using enable_if_map_t = std::enable_if_t<is_map_v<T>>;

    template <typename, typename = void>
    struct has_mapped_type : std::false_type { };

    template <typename T>
    struct has_mapped_type<T, std::void_t<typename T::mapped_type>> : std::true_type { };
    
    template <typename T>
    inline bool constexpr has_mapped_type_v = has_mapped_type<T>::value;

    template <typename T>
    struct add_const_if_ref : type_is<T> { };

    template <typename T>
    struct add_const_if_ref<T&> : type_is<T const&> { };

    template <typename T>
    using add_const_if_ref_t = typename add_const_if_ref<T>::type;

    template <typename T>
    struct add_const_if_ptr : type_is<T> { };

    template <typename T>
    struct add_const_if_ptr<T*> : type_is<T const*> { };

    template <typename T>
    using add_const_if_ptr_t = typename add_const_if_ptr<T>::type;

    template <typename T, typename P0, typename... P1toN>
    struct is_one_of : std::conditional_t<std::is_same_v<T, P0>,
                                          std::true_type,
                                          is_one_of<T, P1toN...>> { };

    template <typename T, typename P0>
    struct is_one_of<T, P0> : std::is_same<T, P0> { };

    template <typename T, typename P0, typename... P1toN>
    inline bool constexpr is_one_of_v = is_one_of<T, P0, P1toN...>::value;

    template <typename, typename>
    struct pair_comparator;

    template <typename K, typename M, template  <typename> typename Compare>
    struct pair_comparator<std::pair<K, M>, Compare<std::pair<K, M>>> {
        bool constexpr operator()(std::pair<K, M> const& left, std::pair<K, M> const& right) const {
            return Compare<K>{}(left.first, right.first);
        }
        bool constexpr operator()(K const& left, std::pair<K, M> const& right) const {
            return Compare<K>{}(left, right.first);
        }
        bool constexpr operator()(std::pair<K, M> const& left, K const& right) const {
            return Compare<K>{}(left.first, right);
        }
    };

    template <typename K, typename M, template  <typename> typename Compare>
    struct pair_comparator<std::pair<K const, M>, Compare<std::pair<K, M>>> {
        bool constexpr operator()(std::pair<K, M> const& left, std::pair<K, M> const& right) const {
            return Compare<K>{}(left.first, right.first);
        }
        bool constexpr operator()(K const& left, std::pair<K, M> const& right) const {
            return Compare<K>{}(left, right.first);
        }
        bool constexpr operator()(std::pair<K, M> const& left, K const& right) const {
            return Compare<K>{}(left.first, right);
        }
    };

    template <typename Compare, typename T, typename U>
    bool equals(T const& left, U const& right) {
        return !Compare{}(left, right) && !Compare{}(right, left);
    }

    template <typename T, typename Compare>
    struct key_compare : type_is<Compare> { };

    template <typename K, typename M, typename Compare>
    struct key_compare<std::pair<K, M>, Compare> : type_is<pair_comparator<std::pair<K, M>, Compare>> { };

    template <typename T, typename Compare>
    using key_compare_t = typename key_compare<T, Compare>::type;

    template <typename T, typename Compare>
    struct value_compare : type_is<Compare> { };

    template <typename K, typename M, typename Compare>
    struct value_compare<std::pair<K,M>, Compare> : type_is<unavailable> { };

    template <typename T, typename Compare>
    using value_compare_t = typename value_compare<T, Compare>::type;

    template <typename T>
    struct value_type : type_is<T> { };

    template <typename K, typename M>
    struct value_type<std::pair<K, M>> : type_is<std::pair<K const, M>> { };

    template <typename T>
    using value_type_t = typename value_type<T>::type;

    template <typename T>
    struct add_const_to_key_if_pair : type_is<T> { };

    template <typename K, typename M>
    struct add_const_to_key_if_pair<std::pair<K, M>> : type_is<std::pair<K const, M>> { };

    template <typename T>
    using add_const_to_key_if_pair_t = typename add_const_to_key_if_pair<T>::type;

    template <std::size_t N, typename... Args>
    decltype(auto) constexpr get(Args&&... args) {
        return std::get<N>(std::forward_as_tuple(args...));
    }

    template <std::size_t N, typename... Args>
    struct nth_type : type_is<remove_cvref_t<decltype(get<N>(std::declval<Args>()...))>> { };

    template <std::size_t N, typename... Args>
    using nth_type_t = typename nth_type<N, Args...>::type;

    struct const_tag { };
    struct reverse_tag { };
    struct non_const_tag { };
    struct non_reverse_tag { };

    template <typename T>
    struct requests_const;

    template <>
    struct requests_const<const_tag> : std::true_type { };
    
    template <>
    struct requests_const<non_const_tag> : std::false_type { };

    template <typename T>
    inline bool constexpr requests_const_v = requests_const<T>::value;

    template <typename T>
    struct requests_reverse;

    template <>
    struct requests_reverse<reverse_tag> : std::true_type { };

    template <>
    struct requests_reverse<non_reverse_tag> : std::false_type { };

    template <typename T>
    inline bool constexpr requests_reverse_v = requests_reverse<T>::value;

    enum class Color { Red, Black };
    enum class Direction { Left, Right };
    enum class ValueRelation { Less = -1, 
                               Equal, 
                               Larger };

    inline Direction operator!(Direction dir) {
        return static_cast<Direction>(!static_cast<std::underlying_type_t<Direction>>(dir));
    }

    inline Direction dir_from_value_rel(ValueRelation rel) {
        return static_cast<Direction>(
                    (static_cast<std::underlying_type_t<ValueRelation>>(rel) + 1) / 2);
    }

    template <typename Value>
    struct node {
        static_assert(!std::is_const_v<std::remove_reference_t<Value>>, 
                      "Value type should never be const");
    
        static unsigned char constexpr RIGHT_THREAD = 0x1;
        static unsigned char constexpr LEFT_THREAD  = 0x2;
        static unsigned char constexpr SENTINEL     = 0x4;
        static unsigned char constexpr LEAF         = LEFT_THREAD | RIGHT_THREAD;

        alignas(Value) unsigned char storage[sizeof(Value)];
        node *left, *right;
        Color color;
        unsigned char flags;
        
        template <typename T = Value, typename = disable_if_same_t<T, node>>
        node(T&& value, node* ln, node* rn, Color col, unsigned char threaded) 
            : left{ln}, right{rn}, color{col}, flags(threaded & LEAF) { 
            new (storage) Value(std::forward<T>(value));
        }

        node(node* ln, node* rn, Color col, unsigned char threaded)
            : left{ln}, right{rn}, color{col}, flags((threaded & LEAF) | SENTINEL) { }

        node(node const& other) 
            : left{other.left}, right{other.right}, color{other.color}, flags{other.flags} {
            if(!(other.flags & SENTINEL))
                new (storage) Value(other.value());
        }
    
        node(node&& other) 
            : left{other.left}, right{other.right}, color{other.color}, flags{other.flags} {
            if(!(other.flags & SENTINEL))
                new (storage) Value(std::move(other.value()));
        }

        node& operator=(node const& other) & {
            if(!(other.flags & SENTINEL))
                new (storage) Value(other.value());
            left   = other.left;
            right  = other.right;
            color  = other.color;
            flags  = other.flags;
            
            return *this;
        }

        node& operator=(node&& other) & {
            if(!(other.flags & SENTINEL))
                new (storage) Value(std::move(other.value()));
            left   = other.left;
            right  = other.right;
            color  = other.color;
            flags  = other.flags;
            
            return *this;
        }

        ~node() {
            if(!(flags & SENTINEL))
                reinterpret_cast<Value*>(storage)->~Value();
        }

        Value& value() {
            return *reinterpret_cast<Value*>(storage);
        }
        
        Value value() const {    
            return *reinterpret_cast<Value const*>(storage);
        }

        bool is_leaf() const {
            return (flags & LEAF) == LEAF;
        }
    
        bool has_left_child() const {
            return !(flags & LEFT_THREAD);
        }

        bool has_right_child() const {
            return !(flags & RIGHT_THREAD);
        }
    
        void set_left_thread() {
            flags |= LEFT_THREAD;
        }

        void set_right_thread() {
            flags |= RIGHT_THREAD;
        }

        void unset_left_thread() {
            flags &= ~LEFT_THREAD;
        }

        void unset_right_thread() {
            flags &= ~RIGHT_THREAD;
        }

    };

    #ifdef TRBT_DEBUG
    namespace debug {
        inline int constexpr THREAD_ERROR = 0x1;
        inline int constexpr LINK_ERROR   = 0x2;
        inline int constexpr NODE_ERROR   = 0x4;

        inline bool constexpr thread_error(int flags) noexcept {
            return flags & THREAD_ERROR;
        }
        
        inline bool constexpr link_error(int flags) noexcept {
            return flags & LINK_ERROR;
        }

        inline bool constexpr node_error(int flags) noexcept {
            return flags & NODE_ERROR;
        }
    
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
        struct thread_link_exception : std::runtime_error {
            using std::runtime_error::runtime_error;
        };

    } /* namespace debug */
    #endif

    template <typename Derived, typename Container, typename ConstTag, typename ReverseTag>
    class iterator_base {
        template <typename, typename>
        friend class const_iterator_type;

        using node_type           = node<typename Container::value_type>;

        /* If Container has mapped_type, reference and pointer should be allowed to modify values,
         * otherwise they should be const */
        using container_reference = std::conditional_t<has_mapped_type_v<Container>,
                                                       typename Container::reference,
                                                       add_const_if_ref_t<typename Container::reference>>;
        using container_pointer   = std::conditional_t<has_mapped_type_v<Container>,
                                                       typename Container::pointer,
                                                       add_const_if_ptr_t<typename Container::pointer>>;
        public:
            using value_type        = typename Container::value_type;
            using difference_type   = std::ptrdiff_t;
            using iterator_category = std::bidirectional_iterator_tag;
            using reference         = std::conditional_t<requests_const_v<ConstTag>, 
                                                         typename Container::const_reference, 
                                                         container_reference>;
            using const_reference   = typename Container::const_reference;
            using pointer           = std::conditional_t<requests_const_v<ConstTag>, 
                                                         typename Container::const_pointer,
                                                         container_pointer>;
            using const_pointer     = typename Container::const_pointer;

            explicit iterator_base(Container* cont) : parent_{cont}, current_{parent_->sentinel_} { }
            iterator_base(Container* cont, node_type* t) : parent_{cont}, current_{t} { }

            Derived& operator=(Derived const& rhs) & {
                this->parent_ = rhs.parent_;
                this->current_ = rhs.current_;
                return static_cast<Derived&>(*this);
            }
        
            friend void swap(Derived& left, Derived& right) noexcept {
                auto temp = std::move(left);
                left = std::move(right);
                right = std::move(temp);
            }

            friend bool operator==(iterator_base const& left, iterator_base const& right) {
                return left.current_ == right.current_;
            }

            friend bool operator!=(iterator_base const& left, iterator_base const& right) {
                return !(left == right);
            }
    
            Derived& operator++() {
                if constexpr(requests_reverse_v<ReverseTag>) 
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
                if constexpr(requests_reverse_v<ReverseTag>)
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

            reference operator*() {
                return this->current_->value();
            }
            
            const_reference operator*() const {
                return this->current_->value();
            }

            pointer operator->() {
                return std::addressof(this->current_->value());
            }
        
            const_pointer operator->() const {
                return std::addressof(this->current_->value());
            }

        protected:
            Container* parent_;
            node_type* current_;
    };

    template <typename, typename>
    class const_iterator_type;
    
    template <typename Container, typename ReverseTag>
    class iterator_type : public iterator_base<iterator_type<Container, ReverseTag>, Container, non_const_tag, ReverseTag> { 
        using base = iterator_base<iterator_type<Container, ReverseTag>, Container, non_const_tag, ReverseTag>;
        public:
            using value_type        = typename base::value_type;
            using difference_type   = typename base::difference_type;
            using iterator_category = typename base::iterator_category;
            using reference         = typename base::reference;
            using const_reference   = typename base::const_reference;
            using pointer           = typename base::pointer;
            using const_pointer     = typename base::const_pointer;

            using base::base;
            iterator_type(iterator_type const& other) : base{other.parent_, other.current_} { }

            operator const_iterator_type<Container, ReverseTag>() {
                return const_iterator_type<Container, ReverseTag>{this->parent_, this->current_};
            }
    };

    template <typename Container, typename ReverseTag>
    class const_iterator_type : public iterator_base<const_iterator_type<Container const, ReverseTag>, Container const, const_tag, ReverseTag> {

        template <typename, typename, typename>
        friend class trbt::rbtree;
        
        using base = iterator_base<const_iterator_type<Container const, ReverseTag>, Container const, const_tag, ReverseTag>;
        public: 
            using value_type        = typename base::value_type;
            using difference_type   = typename base::difference_type;
            using iterator_category = typename base::iterator_category;
            using reference         = typename base::reference;
            using const_reference   = typename base::const_reference;
            using pointer           = typename base::pointer;
            using const_pointer     = typename base::const_pointer;

            using base::base;
    
            const_iterator_type(const_iterator_type const& other) : base{other.parent_, other.current_} { }
            const_iterator_type(iterator_type<Container, ReverseTag> const& other) : base{other.parent_, other.current_} { }

            const_iterator_type& operator=(iterator_type<Container, ReverseTag> const& other) & {
                this->parent_ = other.parent_;
                this->current_ = other.current_;
                return *this;
            }
    };

    template <typename Container>
    using iterator = iterator_type<Container, non_reverse_tag>;
    template <typename Container>
    using reverse_iterator = iterator_type<Container, reverse_tag>;
    template <typename Container>
    using const_iterator = const_iterator_type<Container, non_reverse_tag>;
    template <typename Container>
    using const_reverse_iterator = const_iterator_type<Container, reverse_tag>;

} /* namespace impl */

template <typename Value, 
          typename Compare = std::less<Value>, 
          typename Allocator = std::allocator<impl::add_const_to_key_if_pair<impl::remove_cvref_t<Value>>>>
class rbtree {
    static_assert(impl::is_comparable_v<impl::remove_cvref_t<Value>,
                                        impl::key_compare_t<impl::remove_cvref_t<Value>, Compare>>, 
                                        "Value type is not comparable");

    template <typename, typename, typename, typename>
    friend class impl::iterator_base;

    using Alloc     = typename std::allocator_traits<Allocator>::template 
                                    rebind_alloc<impl::node<impl::value_type_t<impl::remove_cvref_t<Value>>>>;
    using Color         = impl::Color;
    using Direction     = impl::Direction;
    using ValueRelation = impl::ValueRelation;

    #ifdef TRBT_DEBUG
    using color_violation_exception        = impl::debug::color_violation_exception;
    using bst_property_violation_exception = impl::debug::bst_property_violation_exception;
    using height_violation_exception       = impl::debug::height_violation_exception;
    #endif

    public:
        using key_type               = impl::key_type_t<impl::remove_cvref_t<Value>>;
        using mapped_type            = impl::mapped_type_t<impl::remove_cvref_t<Value>>;
        using value_type             = impl::value_type_t<impl::remove_cvref_t<Value>>;
        using size_type              = std::size_t;
        using difference_type        = std::ptrdiff_t;
        using key_compare            = impl::key_compare_t<impl::remove_cvref_t<Value>, Compare>;
        using value_compare          = impl::value_compare_t<impl::remove_cvref_t<Value>, Compare>;
        using allocator_type         = Allocator;
        using reference              = value_type&;
        using const_reference        = value_type const&;
        using pointer                = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer          = typename std::allocator_traits<Allocator>::const_pointer;
        using node_type              = impl::node<value_type>;
        using iterator               = impl::iterator<rbtree>;
        using const_iterator         = impl::const_iterator<rbtree>;
        using reverse_iterator       = impl::reverse_iterator<rbtree>;
        using const_reverse_iterator = impl::const_reverse_iterator<rbtree>;

        rbtree();

        template <typename T = value_type, typename = impl::disable_if_same_t<T, rbtree>>
        rbtree(T&& value);

        /* Value type will be smallest possible type that can store all types of the pack */
        template <typename... Args, typename = std::enable_if_t<(sizeof...(Args) > 1) && (std::is_convertible_v<Args, value_type> && ...)>>
        rbtree(Args&&... values);
        
        template <typename InputIt, typename = impl::enable_if_iterator_t<InputIt>>
        rbtree(InputIt first, InputIt last);

        rbtree(rbtree const& other);
        rbtree(rbtree&& other);

        ~rbtree();

        rbtree& operator=(rbtree const& other) &;
        rbtree& operator=(rbtree&& other) &;

        inline bool empty() const;
        inline size_type size() const noexcept;
        inline size_type max_size() const noexcept;

        void clear() noexcept;

        allocator_type get_allocator() const;

        #ifdef TRBT_DEBUG
        void print() const;
        #endif

        template <typename T = value_type, typename = impl::enable_if_convertible_t<T, value_type>>
        std::pair<iterator, bool> insert(T&& value);
        template <typename InputIt, typename = impl::enable_if_iterator_t<InputIt>>
        void insert(InputIt first, InputIt last);

        template <typename T = value_type, typename = impl::enable_if_convertible_t<T, value_type>>
        iterator insert(const_iterator hint, T&& value);

        template <typename... Args>
        std::pair<iterator, bool> emplace(Args&&... args);

        template <typename... Args>
        iterator emplace_hint(const_iterator hint, Args&&... args);

        size_type erase(value_type const& value);

        bool contains(value_type const& value) const;
        size_type count(value_type const& value) const;
        iterator find(value_type const& value);
        const_iterator find(value_type const& value) const;

        template <typename T = rbtree, typename = impl::enable_if_map_t<T>>
        mapped_type& operator[](key_type const& key);
        template <typename T = rbtree, typename = impl::enable_if_map_t<T>>
        mapped_type& operator[](key_type&& key);
        template <typename T = rbtree, typename = impl::enable_if_map_t<T>>
        mapped_type& at(key_type const& key);
        template <typename T = rbtree, typename = impl::enable_if_map_t<T>>
        mapped_type const& at(key_type const& key) const;

        void swap(rbtree& other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value &&
                                                  std::is_nothrow_swappable<Compare>::value);

        iterator lower_bound(value_type const& value);
        const_iterator lower_bound(value_type const& value) const;

        iterator upper_bound(value_type const& value);
        const_iterator upper_bound(value_type const& value) const;

        #ifdef TRBT_DEBUG
        void assert_properties_ok() const;
        int assert_leftmost_ok() const;
        int assert_rightmost_ok() const;
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
        friend bool operator==(rbtree<Val_, Comp_, Alloc_> const& left, rbtree<Val_, Comp_, Alloc_> const& right);
        template <typename Val_, typename Comp_, typename Alloc_>
        friend bool operator!=(rbtree<Val_, Comp_, Alloc_> const& left, rbtree<Val_, Comp_, Alloc_> const& right);
        template <typename Val_, typename Comp_, typename Alloc_>
        friend bool operator<(rbtree<Val_, Comp_, Alloc_> const& left, rbtree<Val_, Comp_, Alloc_> const& right);
        template <typename Val_, typename Comp_, typename Alloc_>
        friend bool operator<=(rbtree<Val_, Comp_, Alloc_> const& left, rbtree<Val_, Comp_, Alloc_> const& right);
        template <typename Val_, typename Comp_, typename Alloc_>
        friend bool operator>(rbtree<Val_, Comp_, Alloc_> const& left, rbtree<Val_, Comp_, Alloc_> const& right);
        template <typename Val_, typename Comp_, typename Alloc_>
        friend bool operator>=(rbtree<Val_, Comp_, Alloc_> const& left, rbtree<Val_, Comp_, Alloc_> const& right);
        template <typename Val_, typename Comp_, typename Alloc_>
        friend void swap(rbtree<Val_, Comp_, Alloc_>& left, rbtree<Val_, Comp_, Alloc_>& right) noexcept(noexcept(left.swap(right)));

    private:
        node_type* sentinel_;
        node_type* leftmost_{nullptr};
        node_type* rightmost_{nullptr};
        size_type size_{};
        Alloc allocator_{};
        key_compare compare_{};

        template <typename T = value_type>
        inline node_type* allocate_node(T&& value, node_type* ln, node_type* rn, Color col, unsigned char thread);
        inline node_type* allocate_node(node_type* ln, node_type* rn, Color col, unsigned char thread);

        void init(unsigned char thread);
        void clear(node_type* current) noexcept;

        node_type* clone(node_type* pred, node_type* succ, node_type* other);

        node_type* find(value_type const& value, node_type* current) const;

        node_type* link(node_type* node, Direction dir) const;
        static node_type* leftmost(node_type* root);
        static node_type* rightmost(node_type* root);
        static inline node_type* successor(node_type* node);
        static inline node_type* predecessor(node_type* node);

        static node_type* left_rotate(node_type* root, node_type* parent);
        static node_type* right_rotate(node_type* root, node_type* parent);
        static inline node_type* left_right_rotate(node_type* root, node_type* parent);
        static inline node_type* right_left_rotate(node_type* root, node_type* parent);
        static inline node_type* left_left_rotate(node_type* root, node_type* parent);
        static inline node_type* right_right_rotate(node_type* root, node_type* parent);

        void recolor_insert(node_type* current, node_type* parent, node_type* grandparent, node_type* great_grandparent);
        void recolor_remove(Direction dir, node_type* current, node_type*& parent, node_type* grandparent, node_type* sibling);

        void enqueue_as_left_child(node_type* new_node, node_type* current);
        void enqueue_as_right_child(node_type* new_node, node_type* current);

        node_type* enqueue_node(node_type* new_node, Direction enq_dir, node_type* current, node_type* parent, node_type* grandparent, node_type* great_grandparent);

        node_type* dequeue_node(node_type* to_deq, node_type* to_deq_parent, node_type* descendant, node_type* descendant_parent);

        template <typename T>
        ValueRelation insert_position(T const& value, node_type*& current, node_type*& parent, node_type*& grandparent, node_type*& great_grandparent);

        template <typename T = value_type>
        node_type* insert_empty(T&& value);

        template <typename... Args>
        node_type* emplace_empty(Args&&... args);


        template <typename T = value_type>
        std::pair<iterator, bool> insert(T&& value, node_type* current);
        
        template <typename... Args>
        std::pair<iterator, bool> emplace(node_type* current, Args&&... args);
        
        size_type erase(value_type const& value, node_type* current);

        node_type* lower_bound(value_type const& value, node_type* current) const;
        node_type* upper_bound(value_type const& value, node_type* current) const;

        #ifdef TRBT_DEBUG
        void print(node_type* t, unsigned indentation = 0) const;
        #endif

        #ifdef TRBT_DEBUG
        int assert_properties_ok(node_type* t) const;
        #endif
};

/* Deduction guides */
template <typename InputIt, 
          typename Compare = std::less<typename std::iterator_traits<InputIt>::value_type>,
          typename Allocator = std::allocator<typename std::iterator_traits<InputIt>::value_type>>
rbtree(InputIt, InputIt, Compare = Compare{}, Allocator = Allocator{})
    -> rbtree<typename std::iterator_traits<InputIt>::value_type, Compare, Allocator>;

template <typename InputIt, class Allocator>
rbtree(InputIt, InputIt, Allocator)
    -> rbtree<typename std::iterator_traits<InputIt>::value_type,
                      std::less<typename std::iterator_traits<InputIt>::value_type>, Allocator>;

template <typename... Args, 
          typename Compare = std::less<impl::compare_type_t<impl::remove_cvref_t<std::common_type_t<Args...>>>>,
          typename Allocator = std::allocator<std::common_type_t<Args...>>>
rbtree(Args...)
    -> rbtree<std::common_type_t<Args...>, Compare, Allocator>;

template <typename Key,
          typename Mapped,
          typename Compare = std::less<std::pair<Key, Mapped>>,
          typename Allocator = std::allocator<std::pair<Key, Mapped>>>
rbtree(std::pair<Key, Mapped>)
    -> rbtree<std::pair<Key const, Mapped>, Compare, Allocator>;

template <template <typename , typename> typename Pair,
          typename Key,
          typename... Mapped,
          typename Compare = std::less<std::pair<Key, std::common_type_t<Mapped...>>>,
          typename Allocator = std::allocator<std::pair<Key const, std::common_type_t<Mapped...>>>>
rbtree(Pair<Key, Mapped>...)
    -> rbtree<std::pair<Key const, std::common_type_t<Mapped...>>, Compare, Allocator>;


/* Member functions */
template <typename Value, typename Compare, typename Allocator>
rbtree<Value, Compare, Allocator>::rbtree() {
    init(node_type::LEAF);
}

template <typename Value, typename Compare, typename Allocator>
template <typename T, typename>
rbtree<Value, Compare, Allocator>::rbtree(T&& value) {
    init(node_type::LEFT_THREAD);
    sentinel_->right = allocate_node(std::forward<T>(value), sentinel_, sentinel_, Color::Black, node_type::LEAF);
    leftmost_ = rightmost_ = sentinel_->right;
}

template <typename Value, typename Compare, typename Allocator>
template <typename... Args, typename>
rbtree<Value, Compare, Allocator>::rbtree(Args&&... values) {
    init(node_type::LEAF);
    (insert(std::forward<Args>(values)), ...);
}

template <typename Value, typename Compare, typename Allocator>
template <typename InputIt, typename>
rbtree<Value, Compare, Allocator>::rbtree(InputIt first, InputIt last) {
    init(node_type::LEAF);
    insert(first, last);
}

template <typename Value, typename Compare, typename Allocator>
rbtree<Value, Compare, Allocator>::rbtree(rbtree const& other) {
    init(other.sentinel_->flags);
    size_ = other.size_;
    if(!other.sentinel_->is_leaf()) 
        sentinel_->right = clone(sentinel_, sentinel_, other.sentinel_->right);
}

template <typename Value, typename Compare, typename Allocator>
rbtree<Value, Compare, Allocator>::rbtree(rbtree&& other) 
    : sentinel_{other.sentinel_}, leftmost_{other.leftmost_}, rightmost_{other.rightmost_},
      size_{other.size_} {

    /* Reset other to empty state */
    other.init(node_type::LEAF);
    other.size_ = 0u;
}

template <typename Value, typename Compare, typename Allocator>
rbtree<Value, Compare, Allocator>::~rbtree() {
    clear();
    allocator_.deallocate(sentinel_, 1u);
}

template <typename Value, typename Compare, typename Allocator>
rbtree<Value, Compare, Allocator>& 
rbtree<Value, Compare, Allocator>::operator=(rbtree const& other) & {
    auto cpy{other};
    std::swap(sentinel_, cpy.sentinel_);
    std::swap(size_, cpy.size_);
    std::swap(leftmost_, cpy.leftmost_);
    std::swap(rightmost_, cpy.rightmost_);
    return *this;
}

template <typename Value, typename Compare, typename Allocator>
rbtree<Value, Compare, Allocator>& 
rbtree<Value, Compare, Allocator>::operator=(rbtree&& other) & {
    init(node_type::LEAF);
    std::swap(sentinel_, other.sentinel_);
    std::swap(size_, other.size_);
    std::swap(leftmost_, other.leftmost_);
    std::swap(rightmost_, other.rightmost_);
    return *this;
}

template <typename Value, typename Compare, typename Allocator>
bool rbtree<Value, Compare, Allocator>::empty() const {
    return !sentinel_->has_right_child();
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::size_type
rbtree<Value, Compare, Allocator>::size() const noexcept {
    return size_;
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::size_type
rbtree<Value, Compare, Allocator>::max_size() const noexcept {
    return std::allocator_traits<Alloc>::max_size(allocator_);
}

template <typename Value, typename Compare, typename Allocator>
void rbtree<Value, Compare, Allocator>::clear() noexcept {
    if(!empty()) {
        clear(sentinel_->right);
        
        sentinel_->right = sentinel_;
        sentinel_->set_right_thread();
        size_ = 0u;
    }
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::allocator_type 
rbtree<Value, Compare, Allocator>::get_allocator() const {
    return allocator_;
}

#ifdef TRBT_DEBUG
template <typename Value, typename Compare, typename Allocator>
void rbtree<Value, Compare, Allocator>::print() const {
    if(empty())
        std::cerr << "Tree is empty\n";
    else
        print(sentinel_->right);
}
#endif

template <typename Value, typename Compare, typename Allocator>
template <typename T, typename>
std::pair<typename rbtree<Value, Compare, Allocator>::iterator, bool> 
rbtree<Value, Compare, Allocator>::insert(T&& value) {
    if(empty()) {
        node_type* node = insert_empty(std::forward<T>(value));
        
        return {iterator{this, node}, true};
    }
    return insert(std::forward<T>(value), sentinel_->right);
}

template <typename Value, typename Compare, typename Allocator>
template <typename InputIt, typename>
void rbtree<Value, Compare, Allocator>::insert(InputIt first, InputIt last) {
    while(first != last)
        insert(*first++);
}

template <typename Value, typename Compare, typename Allocator>
template <typename T, typename>
typename rbtree<Value, Compare, Allocator>::iterator
rbtree<Value, Compare, Allocator>::insert(const_iterator hint, T&& value) {
    using impl::equals;

    bool suitable_parent = hint.current_->color == Color::Black &&
                           !hint.current_->has_left_child();

    bool lt_parent = compare_(value, hint.current_->value());

    if(hint != end() && suitable_parent && lt_parent) {
        if(equals<key_compare>(value, hint.current_->left->value()))
            return iterator{this, hint.current_->left};;

        node_type* node = allocate_node(std::forward<T>(value), nullptr, nullptr, Color::Red, node_type::LEAF);
        enqueue_as_left_child(node, hint.current_);

        return iterator{this, node};
    }
    
    return insert(std::forward<T>(value)).first;
}

template <typename Value, typename Compare, typename Allocator>
template <typename... Args>
std::pair<typename rbtree<Value, Compare, Allocator>::iterator, bool> 
rbtree<Value, Compare, Allocator>::emplace(Args&&... args) {
    if(empty()) {
        node_type* node = emplace_empty(std::forward<Args>(args)...);

        return {iterator{this, node}, true};
    }
    return emplace(sentinel_->right, std::forward<Args>(args)...);
}

template <typename Value, typename Compare, typename Allocator>
template <typename... Args>
typename rbtree<Value, Compare, Allocator>::iterator
rbtree<Value, Compare, Allocator>::emplace_hint(const_iterator hint, Args&&... args) {
    using impl::equals;

    value_type value{std::forward<Args>(args)...};
    bool suitable_parent = hint.current_->color == Color::Black &&
                           !hint.current_->has_left_child();

    bool lt_parent = compare_(value, hint.current_->value());

    if(hint != end() && suitable_parent && lt_parent) {
        if(equals<Compare>(value, hint.current_->left->value()))
            return iterator{this, hint.current_->left};

        node_type* node = allocate_node(std::move(value), nullptr, nullptr, Color::Red, node_type::LEAF);
        enqueue_as_left_child(node, hint.current_);

        return iterator{this, node};
    }
    
    return emplace(std::move(value)).first;

}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::size_type
rbtree<Value, Compare, Allocator>::erase(value_type const& value) {
    if(empty())
        return 0u;
    
    return erase(value, sentinel_->right);
}

template <typename Value, typename Compare, typename Allocator>
bool rbtree<Value, Compare, Allocator>::contains(value_type const& value) const  {
    if(empty())
        return false;

    node_type* current = sentinel_->right;

    while(true) {
        if(compare_(value, current->value())) {
            if(current->has_left_child())
                current = current->left;
            else
                return false;
        }
        else if(compare_(current->value(), value)) {
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

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::size_type
rbtree<Value, Compare, Allocator>::count(value_type const& value) const {
    return static_cast<size_type>(contains(value));
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::iterator 
rbtree<Value, Compare, Allocator>::find(value_type const& value) {
    if(empty())
        return end();
    
    return iterator{this, find(value, sentinel_->right)};
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::const_iterator 
rbtree<Value, Compare, Allocator>::find(value_type const& value) const {
    if(empty())
        return cend();
    
    return const_iterator{this, find(value, sentinel_->right)};
}

template <typename Value, typename Compare, typename Allocator>
template <typename, typename>
typename rbtree<Value, Compare, Allocator>::mapped_type& 
rbtree<Value, Compare, Allocator>::operator[](key_type const& key) {
    return (*insert(std::pair{key, mapped_type{}}, sentinel_->right)).second;
}

template <typename Value, typename Compare, typename Allocator>
template <typename, typename>
typename rbtree<Value, Compare, Allocator>::mapped_type& 
rbtree<Value, Compare, Allocator>::operator[](key_type&& key) {
    return (*insert(std::pair{std::move(key), mapped_type{}}, sentinel_->right).first).second;
}

template <typename Value, typename Compare, typename Allocator>
template <typename, typename>
typename rbtree<Value, Compare, Allocator>::mapped_type& 
rbtree<Value, Compare, Allocator>::at(key_type const& key) {

    if(auto it = find(std::pair{key, mapped_type{}}); it != end())
        return it->second;

    throw std::out_of_range{"Specified key not in tree"};
}

template <typename Value, typename Compare, typename Allocator>
template <typename, typename>
typename rbtree<Value, Compare, Allocator>::mapped_type const& 
rbtree<Value, Compare, Allocator>::at(key_type const& key) const {

    if(auto it = find(std::pair{key, mapped_type{}}); it != end())
        return it->second;

    throw std::out_of_range{"Specified key not in tree"};
}

template <typename Value, typename Compare, typename Allocator>
void rbtree<Value, Compare, Allocator>::swap(rbtree& other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value &&
                                                        std::is_nothrow_swappable<Compare>::value) {
    std::swap(sentinel_, other.sentinel_);
    std::swap(size_, other.size_);
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::iterator 
rbtree<Value, Compare, Allocator>::lower_bound(value_type const& value) {
    if(empty())
        return end();

    return iterator{this, lower_bound(value, sentinel_->right)};
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::const_iterator 
rbtree<Value, Compare, Allocator>::lower_bound(value_type const& value) const {
    if(empty())
        return end();

    return const_iterator{this, lower_bound(value, sentinel_->right)};
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::iterator 
rbtree<Value, Compare, Allocator>::upper_bound(value_type const& value) {
    if(empty())
        return end();

    return iterator{this, upper_bound(value, sentinel_->right)};
}

template <typename Value, typename Compare, typename Allocator> 
typename rbtree<Value, Compare, Allocator>::const_iterator 
rbtree<Value, Compare, Allocator>::upper_bound(value_type const& value) const {
    if(empty())
        return end();

    return const_iterator{this, upper_bound(value, sentinel_->right)};
}

#ifdef TRBT_DEBUG
template <typename Value, typename Compare, typename Allocator>
void rbtree<Value, Compare, Allocator>::assert_properties_ok() const {
    if(!empty()) {
        if(sentinel_->right->color != Color::Black)
            throw color_violation_exception{"Root is red\n"};
        
        assert_properties_ok(sentinel_->right);
    }
}
#endif

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::iterator 
rbtree<Value, Compare, Allocator>::begin() noexcept {
    return iterator{this, leftmost_};
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::iterator 
rbtree<Value, Compare, Allocator>::end() noexcept {
    return iterator{this};
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::const_iterator 
rbtree<Value, Compare, Allocator>::begin() const noexcept {
    return const_iterator{this, leftmost_};
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::const_iterator 
rbtree<Value, Compare, Allocator>::end() const noexcept {
    return const_iterator{this};
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::const_iterator 
rbtree<Value, Compare, Allocator>::cbegin() const noexcept {
    return const_iterator{this, leftmost_};
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::const_iterator 
rbtree<Value, Compare, Allocator>::cend() const noexcept {
    return const_iterator{this};
}
    
template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::reverse_iterator 
rbtree<Value, Compare, Allocator>::rbegin() noexcept {
    return reverse_iterator{this, rightmost_};
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::reverse_iterator 
rbtree<Value, Compare, Allocator>::rend() noexcept {
    return reverse_iterator{this};
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::const_reverse_iterator 
rbtree<Value, Compare, Allocator>::rbegin() const noexcept {
    return const_reverse_iterator{this, rightmost_};
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::const_reverse_iterator 
rbtree<Value, Compare, Allocator>::rend() const noexcept {
    return const_reverse_iterator{this};
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::const_reverse_iterator 
rbtree<Value, Compare, Allocator>::crbegin() const noexcept {
    return const_reverse_iterator{this, rightmost_};
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::const_reverse_iterator 
rbtree<Value, Compare, Allocator>::crend() const noexcept {
    return const_reverse_iterator{this};
}

template <typename Val_, typename Comp_, typename Alloc_>
bool operator==(rbtree<Val_, Comp_, Alloc_> const& left, rbtree<Val_, Comp_, Alloc_> const& right) {
    using impl::equals;
    
    if(left.size() != right.size())
        return false;

    auto left_it  = std::cbegin(left);
    auto right_it = std::cbegin(right);

    while(left_it != std::cend(left) && right_it != std::cend(right))
        if(!equals<impl::key_compare_t<impl::remove_cvref_t<Val_>, Comp_>>(*left_it++, *right_it++))
            return false;

    return true;
}

template <typename Val_, typename Comp_, typename Alloc_>
bool operator!=(rbtree<Val_, Comp_, Alloc_> const& left, rbtree<Val_, Comp_, Alloc_> const& right) {
    return !(left == right);
}

template <typename Val_, typename Comp_, typename Alloc_>
bool operator<(rbtree<Val_, Comp_, Alloc_> const& left, rbtree<Val_, Comp_, Alloc_> const& right) {
    Comp_ compare{};
    auto left_it  = std::cbegin(left);
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
bool operator>(rbtree<Val_, Comp_, Alloc_> const& left, rbtree<Val_, Comp_, Alloc_> const& right) {
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
bool operator<=(rbtree<Val_, Comp_, Alloc_> const& left, rbtree<Val_, Comp_, Alloc_> const& right) {
    return !(left > right);
}

template <typename Val_, typename Comp_, typename Alloc_>
bool operator>=(rbtree<Val_, Comp_, Alloc_> const& left, rbtree<Val_, Comp_, Alloc_> const& right) {
    return !(left < right);
}

template <typename Val_, typename Comp_, typename Alloc_>
void swap(rbtree<Val_, Comp_, Alloc_>& left, rbtree<Val_, Comp_, Alloc_>& right) noexcept(noexcept(left.swap(right))) {
    left.swap(right);
}

#ifdef TRBT_DEBUG
template <typename Value, typename Compare, typename Allocator>
void rbtree<Value, Compare, Allocator>::print(node_type* t, unsigned indentation) const {
    if(t->has_right_child())
        print(t->right, indentation + 2);
    
    std::string const color = t->color == Color::Red ? "R:" : "B:";

    if constexpr(impl::is_map_v<rbtree>) {
        std::cout << color << std::setw(indentation) << ""
                  << "{" << t->value().first << ", " << t->value().second << "}\n";
    }
    else 
        std::cout << color << std::setw(indentation) << "" << t->value() << "\n";

    if(t->has_left_child())
        print(t->left, indentation + 2);
}
#endif

template <typename Value, typename Compare, typename Allocator>
template <typename T>
typename rbtree<Value, Compare, Allocator>::node_type*
rbtree<Value, Compare, Allocator>::allocate_node(T&& value, node_type* ln, node_type* rn, Color col, unsigned char thread) {
    node_type* node = allocator_.allocate(1u);
    node = new (node) node_type{std::forward<T>(value), ln, rn, col, thread};

    return node;
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::node_type* 
rbtree<Value, Compare, Allocator>::allocate_node(node_type* ln, node_type* rn, Color col, unsigned char thread) {
    node_type* node = allocator_.allocate(1u);
    node = new (node) node_type(ln, rn, col, thread);

    return node;
}

template <typename Value, typename Compare, typename Allocator>
void rbtree<Value, Compare, Allocator>::init(unsigned char thread) {
    sentinel_ = allocate_node(nullptr, nullptr, Color::Black, thread);
    sentinel_->left = sentinel_;

    if(sentinel_->is_leaf())
        sentinel_->right = sentinel_;
    else
        size_++;
    
    leftmost_ = rightmost_ = sentinel_;
}

template <typename Value, typename Compare, typename Allocator>
void rbtree<Value, Compare, Allocator>::clear(node_type* current) noexcept {
    if(current->has_left_child())
        clear(current->left);
    if(current->has_right_child())
        clear(current->right);
    
    allocator_.deallocate(current, 1u);
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::node_type* 
rbtree<Value, Compare, Allocator>::clone(node_type* pred, node_type* succ, node_type* other) {

    node_type* node = allocate_node(other->value(), pred, succ, other->color, other->flags);

    if(other->has_left_child())
        node->left = clone(pred, node, other->left);
    else if(leftmost_ == sentinel_ || compare_(node->value(), leftmost_->value()))
        leftmost_ = node;
    if(other->has_right_child())
        node->right = clone(node, succ, other->right);
    else if(rightmost_ == sentinel_ || compare_(rightmost_->value(), node->value()))
        rightmost_ = node;

    return node;
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::node_type*
rbtree<Value, Compare, Allocator>::find(value_type const& value, node_type* current) const {
    while(true) {
        if(compare_(value, current->value())) {
            if(current->has_left_child())
                current = current->left;
            else
                return sentinel_;
        }
        else if(compare_(current->value(), value)) {
            if(current->has_right_child())
                current = current-> right;
            else
                return sentinel_;
        }
        else break;
    }
    return current;
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::node_type*
rbtree<Value, Compare, Allocator>::link(node_type* node, Direction dir) const {
    if(dir == Direction::Right)
        return node->has_right_child() ? node->right : sentinel_;
    
    return node->has_left_child() ? node->left : sentinel_;
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::node_type*
rbtree<Value, Compare, Allocator>::leftmost(node_type* root) {
    while(root->has_left_child())
        root = root->left;

    return root;
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::node_type*
rbtree<Value, Compare, Allocator>::rightmost(node_type* root) {
    while(root->has_right_child())
        root = root->right;

    return root;
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::node_type*
rbtree<Value, Compare, Allocator>::successor(node_type* node) {
    return node->has_right_child() ? leftmost(node->right) : node->right;
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::node_type*
rbtree<Value, Compare, Allocator>::predecessor(node_type* node) {
    return node->has_left_child() ? rightmost(node->left) : node->left;
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::node_type*
rbtree<Value, Compare, Allocator>::left_rotate(node_type* root, node_type* parent) {
    node_type* new_root = root->right;

    if(new_root->has_left_child())
        root->right = new_root->left;
    else {
        root->set_right_thread();
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

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::node_type*
rbtree<Value, Compare, Allocator>::right_rotate(node_type* root, node_type* parent) {
    node_type* new_root = root->left;

    if(new_root->has_right_child())
        root->left = new_root->right;
    else {
        root->set_left_thread();
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

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::node_type*
rbtree<Value, Compare, Allocator>::left_right_rotate(node_type* root, node_type* parent) {
    left_rotate(root->left, root);
    return right_rotate(root, parent);
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::node_type*
rbtree<Value, Compare, Allocator>::right_left_rotate(node_type* root, node_type* parent) {
    right_rotate(root->right, root);
    return left_rotate(root, parent);
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::node_type*
rbtree<Value, Compare, Allocator>::left_left_rotate(node_type* root, node_type* parent) {
    right_rotate(root->left, root);
    return right_rotate(root, parent);
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::node_type*
rbtree<Value, Compare, Allocator>::right_right_rotate(node_type* root, node_type* parent) {
    left_rotate(root->right, root);
    return left_rotate(root, parent);
}

template <typename Value, typename Compare, typename Allocator>
template <typename T>
impl::ValueRelation rbtree<Value, Compare, Allocator>::insert_position(T const& value, node_type*& current, node_type*& parent, node_type*& grandparent, node_type*& great_grandparent) {

    static_assert(std::is_convertible_v<value_type, impl::remove_cvref_t<T>> ||
                  std::is_same_v<node_type, std::remove_pointer_t<T>>);

    Direction dir;
    ValueRelation relation;

    auto auto_compare = [](auto const& left, auto const& right) {
        Compare comp;
        if constexpr(std::is_same_v<impl::remove_cvref_t<decltype(left)>, node_type>) {
            if constexpr(std::is_same_v<impl::remove_cvref_t<decltype(right)>, node_type>) 
                return comp(left.value(), right.value());
            else
                return comp(left.value(), right);
        }
        else {
            if constexpr(std::is_same_v<impl::remove_cvref_t<decltype(right)>, node_type>)
                return comp(left, right.value());
            else
                return comp(left, right);
        }
    };

    while(true) {
        if(link(current, Direction::Left)->color == Color::Red && link(current, Direction::Right)->color == Color::Red)
            recolor_insert(current, parent, grandparent, great_grandparent);

        great_grandparent = grandparent;
        grandparent = parent;
        parent = current;

    
        if(auto_compare(value, current->value())) {
            if(!current->has_left_child()) {
                relation = ValueRelation::Less;
                break;
            }
            else
                dir = Direction::Left;
        }
        else if(auto_compare(current->value(), value)) {
            if(!current->has_right_child()) {
                relation = ValueRelation::Larger;
                break;
            }
            else
                dir = Direction::Right;
        }
        else {
            sentinel_->right->color = Color::Black;
            return ValueRelation::Equal;
        }

        current = link(current, dir);
    }

    sentinel_->right->color = Color::Black;
    return relation;
}

template <typename Value, typename Compare, typename Allocator>
template <typename T>
typename rbtree<Value, Compare, Allocator>::node_type*
rbtree<Value, Compare, Allocator>::insert_empty(T&& value) {
    sentinel_->right = allocate_node(std::forward<T>(value), sentinel_, sentinel_, 
                                   Color::Black, node_type::LEAF);
    sentinel_->unset_right_thread();

    ++size_;

    leftmost_ = rightmost_ = sentinel_->right;

    return sentinel_->right;
}

template <typename Value, typename Compare, typename Allocator>
template <typename... Args>
typename rbtree<Value, Compare, Allocator>::node_type*
rbtree<Value, Compare, Allocator>::emplace_empty(Args&&... args) {
    sentinel_->right = allocate_node(value_type{std::forward<Args>(args)...}, sentinel_, sentinel_, 
                                   Color::Black, node_type::LEAF);
    sentinel_->unset_right_thread();

    ++size_;
    leftmost_ = rightmost_ = sentinel_->right;
    
    return sentinel_->right;
}

template <typename Value, typename Compare, typename Allocator>
void rbtree<Value, Compare, Allocator>::recolor_insert(node_type* current, node_type* parent, node_type* grandparent, node_type* great_grandparent) {
    current->color = Color::Red;

    if(current->has_left_child() && current->has_right_child())
        current->left->color = current->right->color = Color::Black;

    /* parent and grandparent are red, have to rotate */
    if(parent->color == Color::Red) {
        grandparent->color = Color::Red;

        bool current_is_left_child = compare_(current->value(), parent->value());
        /* Nodes are on a line, double rotation */
        if(current_is_left_child != compare_(current->value(), grandparent->value())) {
            if(current_is_left_child)
                right_left_rotate(grandparent, great_grandparent);
            else
                left_right_rotate(grandparent, great_grandparent);

            current->color = Color::Black;
        }
        /* Single rotation */
        else {
            if(compare_(parent->value(), grandparent->value()))
                right_rotate(grandparent, great_grandparent);
            else
                left_rotate(grandparent, great_grandparent);

            parent->color = Color::Black;
        }
    }
}

template <typename Value, typename Compare, typename Allocator>
void rbtree<Value, Compare, Allocator>::recolor_remove(Direction dir, node_type* current, node_type*& parent, node_type* grandparent, node_type* sibling) {

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

    sentinel_->color = Color::Black;
}

template <typename Value, typename Compare, typename Allocator>
void rbtree<Value, Compare, Allocator>::enqueue_as_left_child(node_type* new_node, node_type* parent) {

    parent->unset_left_thread();
    new_node->left = parent->left;
    new_node->right = parent;
    parent->left = new_node;

    if(compare_(new_node->value(), leftmost_->value()))
        leftmost_ = parent->left;

    ++size_;
}

template <typename Value, typename Compare, typename Allocator>
void rbtree<Value, Compare, Allocator>::enqueue_as_right_child(node_type* new_node, node_type* parent) {
    parent->unset_right_thread();
    new_node->left = parent;
    new_node->right = parent->right;
    parent->right = new_node;

    if(compare_(rightmost_->value(), new_node->value()))
        rightmost_ = parent->right;

    ++size_;
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::node_type*
rbtree<Value, Compare, Allocator>::enqueue_node(node_type* new_node, Direction enq_dir, 
                                                node_type* current, node_type* parent, 
                                                node_type* grandparent, 
                                                node_type* great_grandparent) 
{

    if(enq_dir == Direction::Left) 
        enqueue_as_left_child(new_node, current);
    else 
        enqueue_as_right_child(new_node, current);

    current = link(current, enq_dir);

    recolor_insert(current, parent, grandparent, great_grandparent);
    sentinel_->right->color = Color::Black;

    return current;
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::node_type*
rbtree<Value, Compare, Allocator>::dequeue_node(node_type* to_deq, node_type* to_deq_parent, node_type* descendant, node_type* descendant_parent) {
    
    if(to_deq == leftmost_) 
        leftmost_ = successor(leftmost_);
    if(to_deq == rightmost_) 
        rightmost_ = predecessor(rightmost_);

    /* Dequeueing a leaf */
    if(to_deq->is_leaf()) {
        if(to_deq_parent->left == to_deq) {
            to_deq_parent->left = to_deq->left;
            to_deq_parent->set_left_thread();
        }
        else {
            to_deq_parent->right = to_deq->right;
            to_deq_parent->set_right_thread();
        }
    }
    /* 2 children, replace to_deq with descendant and unlink the former */
    else if(to_deq->has_left_child() && to_deq->has_right_child()) {

        if(descendant_parent->left == descendant) { /* equiv. to to_deq == descendant_parent */
            to_deq->left = descendant->left;

            /* Set to_deq left thread if descendant left thread is set */
            to_deq->flags |= (descendant->flags & node_type::LEFT_THREAD);
        }
        else if(descendant->has_left_child())
            descendant_parent->right = descendant->left;

        /* descendant is leaf and descendant_parent == to_deq */
        else
            descendant_parent->set_right_thread();

        /* Make sure descendant matches to_deq */
        descendant->flags = to_deq->flags;
        descendant->color  = to_deq->color;
        descendant->left   = to_deq->left;
        descendant->right  = to_deq->right;

        auto* succ = successor(to_deq);
        if(succ != sentinel_)
            succ->left = descendant;

        /* Replace */
        if(to_deq_parent->left == to_deq)
            to_deq_parent->left = descendant;
        else
            to_deq_parent->right = descendant;
    }
    /* Node to dequeue has exactly 1 child */
    else {
        node_type* child = to_deq->has_left_child() ? to_deq->left : to_deq->right;

        /* Avoid color voilations */
        child->color = to_deq->color;
        
        if(to_deq_parent->left == to_deq)
            to_deq_parent->left = child;
        else
            to_deq_parent->right = child;

        node_type *pred = predecessor(to_deq), *succ = successor(to_deq);

        if(to_deq->has_left_child())
            pred->right = succ;
        else
            succ->left = pred;
    }

    return to_deq;
}

template <typename Value, typename Compare, typename Allocator>
template <typename T>
std::pair<typename rbtree<Value, Compare, Allocator>::iterator, bool> 
rbtree<Value, Compare, Allocator>::insert(T&& value, node_type* current) {

    node_type *parent = sentinel_, *grandparent = sentinel_, *great_grandparent = sentinel_;

    auto relation = insert_position(value, current, parent, grandparent, great_grandparent);
    
    if(relation == ValueRelation::Equal)
        return {iterator{this, current}, false};

    node_type* new_node = allocate_node(std::forward<T>(value), nullptr, nullptr, 
                                        Color::Red, node_type::LEAF);

    node_type* node = enqueue_node(new_node, dir_from_value_rel(relation), current, 
                                   parent, grandparent, great_grandparent);

    return {iterator{this, node}, true};
}

template <typename Value, typename Compare, typename Allocator>
template <typename... Args>
std::pair<typename rbtree<Value, Compare, Allocator>::iterator, bool> 
rbtree<Value, Compare, Allocator>::emplace(node_type* current, Args&&... args) {
    using zeroth_type = impl::remove_cvref_t<impl::nth_type_t<0, Args...>>;

    node_type *parent = sentinel_, *grandparent = sentinel_, *great_grandparent = sentinel_;

    node_type* new_node;

    if constexpr(sizeof...(Args) == 1 && std::is_same_v<zeroth_type, value_type>) {
        new_node = allocate_node(std::forward<Args>(args)..., nullptr, nullptr, 
                                 Color::Red, node_type::LEAF);
    }
    else {
        new_node = allocate_node(value_type{std::forward<Args>(args)...}, nullptr, nullptr, 
                                 Color::Red, node_type::LEAF);
    }
    
    ValueRelation relation = insert_position(*new_node, current, parent, grandparent, 
                                              great_grandparent);
    if(relation == ValueRelation::Equal) {
        allocator_.deallocate(new_node, 1u);
        return {iterator{this, current}, false};    
    }

    node_type* node = enqueue_node(new_node, dir_from_value_rel(relation),
                                   current, parent, grandparent, great_grandparent);

    return {iterator{this, node}, true};
}


template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::size_type
rbtree<Value, Compare, Allocator>::erase(value_type const& value, node_type* current) {
    using impl::equals;

    node_type *parent = sentinel_, *grandparent = sentinel_, *sibling = sentinel_;
    node_type *found = nullptr, *found_parent = nullptr;
    
    Direction dir;
    
    while(true) {
        dir = static_cast<Direction>(compare_(current->value(), value));
        
        /* Ensure node to remove is red */
        if(current->color == Color::Black && link(current, dir)->color == Color::Black) {
            recolor_remove(dir, current, parent, grandparent, sibling);

            /* Ensure rotations haven't separated found and found_parent */
            if(found_parent && found_parent->right != found && found_parent->left != found) {
                if(found_parent == sentinel_)
                    found_parent = sentinel_->right;
                else 
                    found_parent = link(found_parent, static_cast<Direction>(
                                           compare_(found_parent->value(), found->value())));
            }
        }

        /* Correct node found, store and keep moving down */
        if(!found && equals<key_compare>(current->value(), value)) {
            found = current;
            found_parent = parent;
        }
        
        if(link(current, dir) == sentinel_)
            break;

        grandparent = parent;
        parent      = current;
        sibling     = link(current, !dir);
        current     = link(current, dir);
    } 

    size_type deleted = 0u;

    if(found) {
        allocator_.deallocate(dequeue_node(found, found_parent, current, parent), 1u);
        --size_;
        ++deleted;
    }

    if(!empty())
        sentinel_->right->color = Color::Black;

    return deleted;
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::node_type*
rbtree<Value, Compare, Allocator>::lower_bound(value_type const& value, node_type* current) const {
    while(true) {
        if(compare_(value, current->value())) {
            if(current == sentinel_)
                break;

            current = current->left;
        }
        else if(compare_(current->value(), value)) {
            node_type* succ = successor(current);
            if(succ == sentinel_)
                break;
            else if(compare_(value, succ->value()))
                return succ;

            current = current->right;
        }
        else
            return current;
    }
    return sentinel_;
}

template <typename Value, typename Compare, typename Allocator>
typename rbtree<Value, Compare, Allocator>::node_type*
rbtree<Value, Compare, Allocator>::upper_bound(value_type const& value, node_type* current) const {
    while(true) {
        if(compare_(value, current->value())) {
            if(current == sentinel_)
                break;

            current = current->left;
        }
        else if(compare_(current->value(), value)) {
            node_type* succ = successor(current);
            if(succ == sentinel_)
                break;
            else if(compare_(value, succ->value()))
                return succ;

            current = current->right;
        }
        else
            return successor(current);
    }

    return sentinel_;
}


            
#ifdef TRBT_DEBUG
template <typename Value, typename Compare, typename Allocator>
int rbtree<Value, Compare, Allocator>::assert_properties_ok(node_type* t) const {
    node_type *lh = link(t, Direction::Left), *rh = link(t, Direction::Right);
    
    if(t->color == Color::Red) {
        if(lh->color == Color::Red || rh->color == Color::Red)
            throw color_violation_exception{"Node " + std::to_string(t->value()) + 
                                            " is red and has red children\n"};
    }

    if(lh != sentinel_ && !compare_(lh->value(), t->value())) {
        throw bst_property_violation_exception{"Bst property violated by node " + 
                                                std::to_string(t->value()) + 
                                               " and its left child " + 
                                                std::to_string(lh->value()) + "\n"};
    }
    if(rh != sentinel_ && !compare_(t->value(), rh->value())) {
        throw bst_property_violation_exception{"BST property violated by node " +
                                                std::to_string(t->value()) + 
                                               " and its right child " + 
                                                std::to_string(t->right->value())};
    }

    int height_contribution = t->color == Color::Black ? 1 : 0;

    if(t->has_left_child() && t->has_right_child()) {
        int left_height = assert_properties_ok(t->left);
        int right_height = assert_properties_ok(t->right);

        if(left_height != right_height)
            throw height_violation_exception{"Node " + std::to_string(t->value()) + 
                                             ":\nleft height: " + std::to_string(left_height) + 
                                             "\nright height: " + std::to_string(right_height) + 
                                             "\n"};

        return left_height + height_contribution;
    }
    else if(!t->is_leaf()) {
        if(!t->has_left_child()) {
            if(assert_properties_ok(t->right) != -1)
                throw height_violation_exception{"Node " + std::to_string(t->value()) + 
                                "'s right subtree has a black height larger than the left\n"};
        }
        else if(!t->has_right_child()) {
            if(assert_properties_ok(t->left) != -1)
                throw height_violation_exception{"Node " + std::to_string(t->value()) + 
                                "'s left subtree has a black height larger then the right\n"};
        }
    }
    return height_contribution - 1;
}

template <typename Value, typename Compare, typename Allocator>
int rbtree<Value, Compare, Allocator>::assert_leftmost_ok() const {
    using namespace impl::debug;

    if(empty()) {
        return 0;
    }

    auto* ll = leftmost(sentinel_->right);

    int flags = 0;
    if(leftmost_->has_left_child())
        flags |= THREAD_ERROR;
    if(leftmost_->left != sentinel_)
        flags |= LINK_ERROR;
    if(leftmost_ != ll)
        flags |= NODE_ERROR;

    return flags;
}

template <typename Value, typename Compare, typename Allocator>
int rbtree<Value, Compare, Allocator>::assert_rightmost_ok() const {
    using namespace impl::debug;

    if(empty())
        return 0;

    auto* rr = rightmost(sentinel_->right);

    int flags = 0;
    if(rightmost_->has_right_child())
        flags |= THREAD_ERROR;
    if(rightmost_->right != sentinel_)
        flags |= LINK_ERROR;
    if(rightmost_ != rr)
        flags |= NODE_ERROR;

    return flags;
}
#endif

} /* namespace trbt */

#endif
