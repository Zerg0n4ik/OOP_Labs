#include <gtest/gtest.h>
#include "lab1_utils.h"

TEST(GCDTest, BasicCases) {
    EXPECT_EQ(maxDiv(12, 18), 6);
    EXPECT_EQ(maxDiv(18, 12), 6); 
}

TEST(GCDTest, PrimeNumbers) {
    EXPECT_EQ(maxDiv(17, 19), 1);
    EXPECT_EQ(maxDiv(13, 17), 1);
}

TEST(GCDTest, SameNumbers) {
    EXPECT_EQ(maxDiv(15, 15), 15);
    EXPECT_EQ(maxDiv(1, 1), 1);
}

TEST(GCDTest, EdgeCases) {
    EXPECT_EQ(maxDiv(1, 100), 1);
    EXPECT_EQ(maxDiv(0, 5), 0);  
}
