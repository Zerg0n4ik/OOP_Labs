#include <gtest/gtest.h>
#include "../include/lab5.hpp"
#include <vector>

TEST(FixedBlockMemoryResourceTest, BasicAllocation) {
    FixedBlockMemoryResource memory(1024);
    
    void* ptr1 = memory.allocate(64, 8);
    EXPECT_NE(ptr1, nullptr);
    
    void* ptr2 = memory.allocate(128, 16);
    EXPECT_NE(ptr2, nullptr);
    
    memory.deallocate(ptr1, 64, 8);
    memory.deallocate(ptr2, 128, 16);
}

TEST(FixedBlockMemoryResourceTest, ReuseMemory) {
    FixedBlockMemoryResource memory(256);
    
    void* ptr1 = memory.allocate(100, 8);
    EXPECT_NE(ptr1, nullptr);
    
    memory.deallocate(ptr1, 100, 8);
    
    void* ptr2 = memory.allocate(100, 8);
    EXPECT_NE(ptr2, nullptr);
    
    memory.deallocate(ptr2, 100, 8);
}

TEST(PmrDynamicArrayTest, EmptyArray) {
    FixedBlockMemoryResource memory(1024);
    std::pmr::polymorphic_allocator<int> alloc(&memory);
    PmrDynamicArray<int> array(alloc);
    
    EXPECT_TRUE(array.empty());
    EXPECT_EQ(array.size(), 0);
    EXPECT_GE(array.get_capacity(), 0);
}

TEST(PmrDynamicArrayTest, PushBackInt) {
    FixedBlockMemoryResource memory(1024);
    std::pmr::polymorphic_allocator<int> alloc(&memory);
    PmrDynamicArray<int> array(alloc);
    
    array.push_back(1);
    array.push_back(2);
    array.push_back(3);
    
    EXPECT_FALSE(array.empty());
    EXPECT_EQ(array.size(), 3);
    EXPECT_GE(array.get_capacity(), 3);
    EXPECT_EQ(array.front(), 1);
    EXPECT_EQ(array.back(), 3);
}

TEST(PmrDynamicArrayTest, PopBackInt) {
    FixedBlockMemoryResource memory(1024);
    std::pmr::polymorphic_allocator<int> alloc(&memory);
    PmrDynamicArray<int> array(alloc);
    
    array.push_back(1);
    array.push_back(2);
    array.push_back(3);
    
    EXPECT_EQ(array.size(), 3);
    EXPECT_EQ(array.back(), 3);
    
    array.pop_back();
    EXPECT_EQ(array.size(), 2);
    EXPECT_EQ(array.back(), 2);
    
    array.pop_back();
    EXPECT_EQ(array.size(), 1);
    EXPECT_EQ(array.back(), 1);
    
    array.pop_back();
    EXPECT_TRUE(array.empty());
}

TEST(PmrDynamicArrayTest, IteratorInt) {
    FixedBlockMemoryResource memory(1024);
    std::pmr::polymorphic_allocator<int> alloc(&memory);
    PmrDynamicArray<int> array(alloc);
    
    array.push_back(1);
    array.push_back(2);
    array.push_back(3);
    
    std::vector<int> result;
    for (auto it = array.begin(); it != array.end(); ++it) {
        result.push_back(*it);
    }
    
    EXPECT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[1], 2);
    EXPECT_EQ(result[2], 3);
}

TEST(PmrDynamicArrayTest, RangeBasedForLoop) {
    FixedBlockMemoryResource memory(1024);
    std::pmr::polymorphic_allocator<int> alloc(&memory);
    PmrDynamicArray<int> array(alloc);
    
    array.push_back(10);
    array.push_back(20);
    array.push_back(30);
    
    int sum = 0;
    for (const auto& item : array) {
        sum += item;
    }
    
    EXPECT_EQ(sum, 60);
}

TEST(PmrDynamicArrayTest, IndexOperator) {
    FixedBlockMemoryResource memory(1024);
    std::pmr::polymorphic_allocator<int> alloc(&memory);
    PmrDynamicArray<int> array(alloc);
    
    array.push_back(1);
    array.push_back(2);
    array.push_back(3);
    
    EXPECT_EQ(array[0], 1);
    EXPECT_EQ(array[1], 2);
    EXPECT_EQ(array[2], 3);
    
    array[1] = 20;
    EXPECT_EQ(array[1], 20);
}

TEST(PmrDynamicArrayTest, ComplexType) {
    FixedBlockMemoryResource memory(2048);
    std::pmr::polymorphic_allocator<Example> alloc(&memory);
    PmrDynamicArray<Example> array(alloc);
    
    Example e1(1, 2);
    Example e2(3, 4);
    Example e3(5, 6);
    
    array.push_back(e1);
    array.push_back(e2);
    array.push_back(e3);
    
    EXPECT_EQ(array.size(), 3);
    EXPECT_EQ(array.front().x, 1);
    EXPECT_EQ(array.front().y, 2);
    EXPECT_EQ(array.back().x, 5);
    EXPECT_EQ(array.back().y, 6);
    
    array.pop_back();
    EXPECT_EQ(array.back().x, 3);
    EXPECT_EQ(array.back().y, 4);
}

TEST(PmrDynamicArrayTest, IteratorComplexType) {
    FixedBlockMemoryResource memory(2048);
    std::pmr::polymorphic_allocator<Example> alloc(&memory);
    PmrDynamicArray<Example> array(alloc);
    
    array.push_back(Example(1, 10));
    array.push_back(Example(2, 20));
    array.push_back(Example(3, 30));
    
    int count = 0;
    for (const auto& item : array) {
        EXPECT_EQ(item.x, count + 1);
        EXPECT_EQ(item.y, (count + 1) * 10);
        count++;
    }
    EXPECT_EQ(count, 3);
}

TEST(PmrDynamicArrayTest, ResizeCapacity) {
    FixedBlockMemoryResource memory(2048);
    std::pmr::polymorphic_allocator<int> alloc(&memory);
    PmrDynamicArray<int> array(alloc);
    
    size_t initial_capacity = array.get_capacity();
    
    // Добавляем больше элементов, чем начальная емкость
    for (int i = 0; i < 10; ++i) {
        array.push_back(i);
    }
    
    EXPECT_EQ(array.size(), 10);
    EXPECT_GT(array.get_capacity(), initial_capacity);
    EXPECT_GE(array.get_capacity(), 10);
    
    // Проверяем, что все элементы на месте после ресайза
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(array[i], i);
    }
}

TEST(PmrDynamicArrayTest, ClearOnDestruction) {
    FixedBlockMemoryResource memory(1024);
    
    {
        std::pmr::polymorphic_allocator<int> alloc(&memory);
        PmrDynamicArray<int> array(alloc);
        
        array.push_back(1);
        array.push_back(2);
        array.push_back(3);
    }
    
    // После уничтожения массива можно создать новый
    std::pmr::polymorphic_allocator<int> alloc(&memory);
    PmrDynamicArray<int> newArray(alloc);
    
    for (int i = 0; i < 5; ++i) {
        EXPECT_NO_THROW(newArray.push_back(i));
    }
    EXPECT_EQ(newArray.size(), 5);
}

TEST(PmrDynamicArrayTest, FrontBackOnEmpty) {
    FixedBlockMemoryResource memory(1024);
    std::pmr::polymorphic_allocator<int> alloc(&memory);
    PmrDynamicArray<int> array(alloc);
    
    // front() и back() на пустом массиве должны вызывать assert
    // В тестах мы не можем проверить assert, поэтому просто проверяем, что массив пуст
    EXPECT_TRUE(array.empty());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}