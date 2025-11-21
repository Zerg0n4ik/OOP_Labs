#ifndef LAB5_HPP
#define LAB5_HPP

#include <list>
#include <memory_resource>
#include <iterator>

class FixedBlockMemoryResource : public std::pmr::memory_resource {
private:
    void* buffer;
    size_t capacity;
    std::list<std::pair<void*, size_t>> freeBlocks;
    std::list<std::pair<void*, size_t>> usedBlocks;

protected:
    void* do_allocate(size_t bytes, size_t alignment) override;
    void do_deallocate(void* p, size_t bytes, size_t alignment) override;
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

public:
    explicit FixedBlockMemoryResource(size_t capacityBytes);
    ~FixedBlockMemoryResource();
};

template <class T, class Alloc = std::pmr::polymorphic_allocator<T>>
class PmrDynamicArray {
private: 
    T* data = nullptr;
    size_t count = 0;
    size_t capacity = 0;
    Alloc allocator;

    void resize(size_t new_capacity);

public:
    explicit PmrDynamicArray(const Alloc& alloc = Alloc());
    ~PmrDynamicArray();

    bool empty() const;
    size_t size() const;
    size_t get_capacity() const;

    void push_back(const T& value);
    void pop_back();

    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    T& front();
    T& back();

    class iterator {
        T* ptr;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using reference = T&;
        iterator(T* p = nullptr) : ptr(p) {}

        reference operator*() const { 
            return *ptr;
        }
        iterator& operator++() {
            ptr++; return *this; 
        }

        bool operator!=(const iterator& other) const { 
            return ptr != other.ptr; 
        }

        bool operator==(const iterator& other) const { 
            return ptr == other.ptr; 
        }
    };

    iterator begin();
    iterator end();
};

struct Example {
    int x, y;
    Example(int x = 0, int y = 0) : x(x), y(y) {}
};

#endif