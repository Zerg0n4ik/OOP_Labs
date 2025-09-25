#include <gtest/gtest.h>
#include "../include/lab1_utils.h"

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
    EXPECT_EQ(maxDiv(0, 5), 5);  
}

TEST(GCDTest, ZeroZero) {
    EXPECT_EQ(maxDiv(0, 0), 0);
    EXPECT_EQ(maxDiv(0, 0), 0);  
}

TEST(GCDTest, ZeroNotZero) {
    EXPECT_EQ(maxDiv(0, 100), 100);
    EXPECT_EQ(maxDiv(10, 0), 10);  
}

TEST(GCDTest, PlusMinus) {
    EXPECT_EQ(maxDiv(-25, 100), 25);
    EXPECT_EQ(maxDiv(25, -5), 5);  
}

TEST(GCDTest, MinusMinus) {
    EXPECT_EQ(maxDiv(-50, -100), 50);
    EXPECT_EQ(maxDiv(-25, -10), 5);  
}