# TRBT
A threaded red-black tree class template.

[![Build Status](https://gitlab.com/vilhelmengstrom/trbt/badges/master/build.svg)](https://gitlab.com/vilhelmengstrom/trbt/commits/master)

The tree provides the majority of functionality available in the standard `set` and `map` containers but as a single class template. The project was written with the aim of exploring slightly more advanced aspects of meta-programming. As such, there are parts that may appear strange and convoluted.

### Behavior
The tree has full support for all comparable and copy assignable types. Move only types may be stored in the tree but there is currently no way to retrieve them from it. It is not required that the type is default constructible.

#### Meta-programming
As mentioned, there is a relatively heavy reliance on meta-programming, making compile times less than optimal. This was a concious choice made during development as the tree was never intended to be used in production. As such, there was no need to try to keep compile times down.  

In addition, functions that are only available for e.g. the pair version (such as operator[]) are disabled for the single value case using SFINAE.

#### Balancing 
Balancing is done in a top-down fashion. This means that the tree will perform the balancing operations during the search for the correct position to perform the insertion or deletion. The correctness of such a strategy can without any larger effort be proven using mathematical induction.  

As a result of the balancing strategy used, each insertion or deletion requires only a single traversal of the tree, generating O(log n) cold cache-misses and O(1) capacity misses. The comparatively more common alternative of first performing the insertion or deletion and then balance the tree requires two separate traversals and as such results in both O(log n) cold misses and O(log n) capacity misses.  

The main con of relying on top-down balancing is that some useful functions provided in std::set and `std::map` (such as the erase function taking a single (const_)iterator) would be more difficult to implement.   

#### Notes on Memory 
Internally, the tree uses a sentinel node to which the actual root of the tree is connected. The sentinel is also used as the element to which `(c)end` and `(c)rend` refer. Dereferincing these iterators is, as usual, undefined behavior.  

In order to use a sentinel node while not requiring that the value type be default constructible, the internal nodes use aligned raw storage (i.e. an array of chars aligned as the value type into which the value is written using perfect forwarding and placement new). In the sentinel node, this memory is never initialized, meaning that the result of accessing its value field (which for users is only possible through dereferencing the `(c)end` and `(c)rend` iterators) is very much undefined.  

#### Iterators
Most of the iterator functionality is implemented in the class template `trbt::iterator_base`. This uses CRTP to return correct value types from its member functions.  

Due to the internal layout of the tree, the `std::reverse_iterator` adapter does not work. Instead, a constexpr check is performed in the increment and decrement operators of `iterator_base`. This results in correct behavior for both the "regular" and the reverse iterators when inheriting from `iterator_base` without incurring any additional runtime cost.

### Tests
Tests for every major member function are available in the tests directory. Each test generates a tree of (pseudo) random size with (pseudo) random content, performs the operation to be tested and then asserts that the correct behavior has been observed. In order to lower the risk of something slipping through the cracks, this procedure is repeated a random number of times for each function.

Currently, the types used when testing are `int`, `std::string` and `std::pair<int, double>`. Which tests to run for which type can be specified in the `tests/trbt_test_config.h` file (note that it is required to recompile after doing so).

The tests stop as soon as an error is encountered (internally, an exception is thrown). This choice was made since it proved helpful to see the conditions under which the error occurred.

#### Tracing
All tests are run using the `trbt_trace_type` rather than the actual `rbtree`. The former is a class template that extends its template parameter and provides a queue to store instances of its base class in. Before the tree is altered through an insertion or deletion, the `trbt_trace_type` enqueues its current state. This way, whenever an error occurs, it is possible to print the previous configurations of the tree to see exactly what what went wrong where. The number of previous configurations the `trbt_trace_type` should store is set in `tests/trbt_test_config.h`.

In order to not have to rely on dynamic polymorphism during the tracing, each test calls a driver function template (e.g. `trace_insert_if_available` rather than the actual `rbtree::insert` member function). This call relies on expression SFINAE to invoke the correct function. This way, the tests work for instances of classes generated from either of the `rbtree` and `trbt_trace_type` templates.
