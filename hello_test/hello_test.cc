#include <gtest/gtest.h>
#include "dreadpools/threadpool.hpp"


int f(int x) {
  return x;
}

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  dreadpools::ThreadPool pool{};
  auto fut = pool.submit(f, 5);
  pool.start();
  const int x = fut.get();
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(x, 5);
}
