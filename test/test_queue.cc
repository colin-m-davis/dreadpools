#include "dreadpools/taskqueue.hpp"

#include <numeric>
#include <vector>
#include <gtest/gtest.h>

namespace dreadpools {
namespace {

/* BEGIN TESTS */

TEST (TestQueue, DefaultCtr) {
    TaskQueue<int> q;
    std::vector nums(32, 0);
    std::iota(nums.begin(), nums.end(), 0);
    for (int x : nums) {
        q.enqueue(x);
    }
    std::vector<int> result;
    while (!q.empty()) {
        int x = 1;
        q.dequeue(x);
        result.push_back(x);
    }
    EXPECT_EQ(nums, result);
}

/* END TESTS */

}
}  // namespace dreadpools


int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
