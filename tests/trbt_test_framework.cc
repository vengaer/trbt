#include "trbt_test_framework.h"
#include <algorithm>
#include <cmath>


namespace trbt {
namespace test {
    std::vector<int> generate_int_vec(std::size_t size) {
        std::mt19937 mt{std::random_device{}()};
        std::vector<int> vals(size);
        std::uniform_int_distribution<> dis(-10 * size, 10 * size);
        std::generate(std::begin(vals), std::end(vals), [&dis, &mt]() {
            return dis(mt);
        });
        std::sort(std::begin(vals), std::end(vals));
        vals.erase(std::unique(std::begin(vals), std::end(vals)), std::end(vals));

        return vals;
    }
    
    std::vector<std::string> generate_string_vec(std::size_t size) {
        std::mt19937 mt{std::random_device{}()};
        std::uniform_real_distribution<> dis(0.0, 1.0);
        auto int_vec = generate_int_vec(size);
        std::vector<std::string> vals(size);

        std::transform(std::begin(int_vec), std::end(int_vec), 
                       std::begin(vals), [&dis, &mt](int const& i) {
            std::string str =  std::to_string(std::abs(i));
            
            /* Map each digit in string to ascii values 65 to 122 */
            std::transform(std::begin(str), std::end(str),
                           std::begin(str), [&dis, &mt](char c) {
                return 66 + (c - 48) * (122 - 65) / 10 - dis(mt);
            });
        
            return str;
        });
        vals.erase(std::unique(std::begin(vals), std::end(vals)), std::end(vals));
        
        return vals;
    }
        
    std::vector<std::pair<int, double>> generate_pair_vec(std::size_t size) {
        auto int_vec = generate_int_vec(size);
        std::vector<std::pair<int, double>> vals(size);
    
        std::transform(std::begin(int_vec), std::end(int_vec),
                       std::begin(vals), [](int const& i) {
            return std::make_pair(i, double{});
        });
        vals.erase(std::unique(std::begin(vals), std::end(vals)), std::end(vals));

        return vals;
    }

    void print_heading(std::string const& method, std::size_t test_size, std::size_t current_iter, std::size_t iterations) {
        std::cout << std::setfill('=') << std::setw(60) << "" << "\n"
                  << "Running " <<  method << " test\nTest size: " << test_size 
                  << "\n" << "Iteration " << ++current_iter << " of " 
                  << iterations << "\n" << std::setw(60) << "" << "\n\n"
                  << std::setfill(' ');
    }
    
} /* namespace test */
} /* namespace tree */
