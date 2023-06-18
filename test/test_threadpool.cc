#include "dreadpools/threadpool.hpp"

#include <string>
#include <vector>
#include <numeric>

#include <gtest/gtest.h>

namespace dreadpools {
namespace {


/* BEGIN HELPER FUNCTIONS */

using namespace std::literals::chrono_literals;

// "identity map"
template <typename T>
T identity(T arg) {
    std::this_thread::sleep_for(1ms);
    return arg;
}

/* END HELPER FUNCTIONS */


/* BEGIN TESTS */

TEST(HelloTest, DefaultCtr) {
    dreadpools::ThreadPool pool{};
    pool.start();

    std::string in_string{"hello dreadpool!"};
    auto fut = pool.submit(identity<decltype(identity(in_string))>, in_string);
    auto out_string = fut.get();

    EXPECT_EQ(in_string, out_string);
}

TEST(HelloTest, Lambda) {
    dreadpools::ThreadPool pool(4);
    pool.start();

    auto expected = 2;
    auto fut = pool.submit(
        [](const int a, const int b) {return std::max(a, b); },
        1, 2
    );
    auto result = fut.get();
    
    EXPECT_EQ(expected, result);
}

TEST(HelloTest, Reduce) {
    dreadpools::ThreadPool pool(4);
    pool.start();

    std::vector<int> in_vec(2048);
    std::iota(in_vec.begin(), in_vec.end(), 0);
    std::vector<int> out_vec;
    auto expected = std::reduce(in_vec.cbegin(), in_vec.cend());
    auto fut = pool.submit(
        std::reduce<decltype(in_vec.cbegin())>,
        in_vec.cbegin(),
        in_vec.cend()
    );
    auto result = fut.get();
    
    EXPECT_EQ(expected, result);
}

TEST(HelloTest, Overload) {
    dreadpools::ThreadPool pool(4);
    pool.start();

    std::vector<int> in_vec(2048);
    std::vector<std::future<int>> fut_vec;
    std::iota(in_vec.begin(), in_vec.end(), 0);
    for (const int x : in_vec) {
        fut_vec.push_back(pool.submit(identity<int>, x));
    }
    std::vector<int> out_vec;
    for (auto& fut : fut_vec) {
        out_vec.push_back(fut.get());
    }

    EXPECT_EQ(in_vec, out_vec);
}

/* END TESTS */

}
}


int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
