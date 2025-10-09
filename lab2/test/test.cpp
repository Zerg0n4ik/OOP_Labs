#include <gtest/gtest.h>
#include <stdexcept>
#include "../include/lab2_utils.h"

TEST(OctalTest, BasicConstruction) {
    Octal empty;
    EXPECT_EQ(empty.toString(), "0");
    
    Octal fromString("123");
    EXPECT_EQ(fromString.toString(), "123");
    
    Octal fromList{1, 2, 3};
    EXPECT_EQ(fromList.toString(), "123");
    
    Octal filled(3, 5);
    EXPECT_EQ(filled.toString(), "555");
}

TEST(OctalTest, StringConstructor) {
    Octal num1("7");
    EXPECT_EQ(num1.toString(), "7");
    
    Octal num2("77");
    EXPECT_EQ(num2.toString(), "77");
    
    Octal num3("777");
    EXPECT_EQ(num3.toString(), "777");
    
    Octal num4("1000");
    EXPECT_EQ(num4.toString(), "1000");
}

TEST(OctalTest, InitializerListConstructor) {
    Octal num1{1};
    EXPECT_EQ(num1.toString(), "1");
    
    Octal num2{1, 2};
    EXPECT_EQ(num2.toString(), "12");
    
    Octal num3{7, 0, 1};
    EXPECT_EQ(num3.toString(), "701");
    
    Octal num4{0, 0, 1, 2};
    EXPECT_EQ(num4.toString(), "12");
}

TEST(OctalTest, AdditionBasic) {
    Octal a("12");
    Octal b("5");
    Octal sum = a.add(b);
    EXPECT_EQ(sum.toString(), "17");
    
    Octal c("7");
    Octal d("1");
    Octal sum2 = c.add(d);
    EXPECT_EQ(sum2.toString(), "10");
}

TEST(OctalTest, AdditionWithCarry) {
    Octal a("777");
    Octal b("1");
    Octal sum = a.add(b);
    EXPECT_EQ(sum.toString(), "1000");
    
    Octal c("77");
    Octal d("7");
    Octal sum2 = c.add(d);
    EXPECT_EQ(sum2.toString(), "106");
}

TEST(OctalTest, SubtractionBasic) {
    Octal a("17");
    Octal b("5");
    Octal diff = a.subtract(b);
    EXPECT_EQ(diff.toString(), "12");
    
    Octal c("100");
    Octal d("1");
    Octal diff2 = c.subtract(d);
    EXPECT_EQ(diff2.toString(), "77");
}

TEST(OctalTest, SubtractionEdgeCases) {
    Octal a("10");
    Octal b("1");
    Octal diff = a.subtract(b);
    EXPECT_EQ(diff.toString(), "7");
    
    Octal c("1000");
    Octal d("777");
    Octal diff2 = c.subtract(d);
    EXPECT_EQ(diff2.toString(), "1");
}

TEST(OctalTest, SubtractionNegativeResult) {
    Octal a("5");
    Octal b("10");
    
    EXPECT_THROW({
        Octal diff = a.subtract(b);
    }, std::invalid_argument);
}

TEST(OctalTest, EqualityComparison) {
    Octal a("123");
    Octal b("123");
    Octal c("124");
    
    EXPECT_TRUE(a.equals(b));
    EXPECT_TRUE(b.equals(a));
    EXPECT_FALSE(a.equals(c));
    EXPECT_FALSE(c.equals(a));
}

TEST(OctalTest, LessThanComparison) {
    Octal a("12");
    Octal b("123");
    Octal c("12");
    
    EXPECT_TRUE(a.lessThan(b));
    EXPECT_FALSE(b.lessThan(a));
    EXPECT_FALSE(a.lessThan(c));
}

TEST(OctalTest, GreaterThanComparison) {
    Octal a("123");
    Octal b("12");
    Octal c("123");
    
    EXPECT_TRUE(a.greaterThan(b));
    EXPECT_FALSE(b.greaterThan(a));
    EXPECT_FALSE(a.greaterThan(c));
}

TEST(OctalTest, ZeroOperations) {
    Octal zero;
    Octal one("1");
    Octal num("123");
    
    Octal sum1 = zero.add(one);
    EXPECT_EQ(sum1.toString(), "1");
    
    Octal sum2 = num.add(zero);
    EXPECT_EQ(sum2.toString(), "123");
    
    Octal diff1 = one.subtract(zero);
    EXPECT_EQ(diff1.toString(), "1");
    
    Octal diff2 = num.subtract(zero);
    EXPECT_EQ(diff2.toString(), "123");
}

TEST(OctalTest, LargeNumbers) {
    Octal a("1000000");
    Octal b("777777");
    Octal c("1");
    
    Octal sum = a.add(b);
    EXPECT_EQ(sum.toString(), "1777777");
    
    Octal diff = a.subtract(c);
    EXPECT_EQ(diff.toString(), "777777");
}

TEST(OctalTest, CopyConstructor) {
    Octal original("12345");
    Octal copy(original);
    
    EXPECT_EQ(original.toString(), copy.toString());
    EXPECT_TRUE(original.equals(copy));
}

TEST(OctalTest, MoveConstructor) {
    Octal original("12345");
    Octal moved(std::move(original));
    
    EXPECT_EQ(moved.toString(), "12345");
}

TEST(OctalTest, InvalidDigits) {
    EXPECT_THROW({
        Octal invalidChar("128"); 
    }, std::invalid_argument);
    
    EXPECT_THROW({
        Octal invalidFill(3, 8); 
    }, std::invalid_argument);
}

TEST(OctalTest, LeadingZerosRemoval) {
    Octal fromString("000123");
    EXPECT_EQ(fromString.toString(), "123");
    
    Octal fromList{0, 0, 0, 1, 2, 3};
    EXPECT_EQ(fromList.toString(), "123");
    
    Octal allZeros{0, 0, 0};
    EXPECT_EQ(allZeros.toString(), "0");
    
    Octal singleZero("0");
    EXPECT_EQ(singleZero.toString(), "0");
}