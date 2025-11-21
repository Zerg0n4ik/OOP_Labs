#include "../include/lab5.hpp"
#include <iostream>
#include <cassert>
#include <algorithm>

FixedBlockMemoryResource::FixedBlockMemoryResource(size_t capacityBytes) : capacity(capacityBytes) {
    buffer = ::operator new(capacity);
    freeBlocks.push_back({buffer, capacity});

    std::cout << "Выделено " << capacity << " байтов.\n";
}

FixedBlockMemoryResource::~FixedBlockMemoryResource() {
    if (!usedBlocks.empty()) {
        std::cerr << "Утечка памяти.\n";
    }
    ::operator delete(buffer);
}

void* FixedBlockMemoryResource::do_allocate(size_t bytes, size_t alignment) {
    auto iter = freeBlocks.begin();
    while (iter != freeBlocks.end()) {
        void* ptr = iter->first;
        size_t size = iter->second;

        void* aligned_ptr = ptr;
        size_t space = size;

        if (std::align(alignment, bytes, aligned_ptr, space)) {
            freeBlocks.erase(iter);
            size_t used = static_cast<char*>(aligned_ptr) - static_cast<char*>(ptr);
            size_t remainingSize = size - used - bytes;
            
            if (used > 0) {
                freeBlocks.push_back({ptr, used});
            }
            if (remainingSize > 0) {
                freeBlocks.push_back({static_cast<char*>(aligned_ptr) + bytes, remainingSize});
            }
            
            usedBlocks.push_back({aligned_ptr, bytes});
            return aligned_ptr;
        }
        ++iter;
    }
    throw std::bad_alloc();
}

void FixedBlockMemoryResource::do_deallocate(void* p, size_t, size_t) {
    auto iter = std::find_if(usedBlocks.begin(), usedBlocks.end(),
        [p](const auto& block) { return block.first == p; });
    
    if (iter != usedBlocks.end()) {
        size_t size = iter->second;
        usedBlocks.erase(iter);
        freeBlocks.push_back({p, size});
    }
}

bool FixedBlockMemoryResource::do_is_equal(
    const std::pmr::memory_resource& other
) const noexcept {
    return this == &other;
}

template <class T, class Alloc>
PmrDynamicArray<T, Alloc>::PmrDynamicArray(const Alloc& alloc) : allocator(alloc) {
    capacity = 4;
    data = allocator.allocate(capacity);
}

template <class T, class Alloc>
PmrDynamicArray<T, Alloc>::~PmrDynamicArray() {
    for (size_t i = 0; i < count; ++i) {
        std::allocator_traits<Alloc>::destroy(allocator, &data[i]);
    }
    allocator.deallocate(data, capacity);
}

template <class T, class Alloc>
bool PmrDynamicArray<T, Alloc>::empty() const { 
    return count == 0; 
}

template <class T, class Alloc>
size_t PmrDynamicArray<T, Alloc>::size() const { 
    return count; 
}

template <class T, class Alloc>
size_t PmrDynamicArray<T, Alloc>::get_capacity() const { 
    return capacity; 
}

template <class T, class Alloc>
void PmrDynamicArray<T, Alloc>::resize(size_t new_capacity) {
    T* new_data = allocator.allocate(new_capacity);
    
    for (size_t i = 0; i < count; ++i) {
        std::allocator_traits<Alloc>::construct(allocator, &new_data[i], std::move(data[i]));
        std::allocator_traits<Alloc>::destroy(allocator, &data[i]);
    }
    
    allocator.deallocate(data, capacity);
    data = new_data;
    capacity = new_capacity;
}

template <class T, class Alloc>
void PmrDynamicArray<T, Alloc>::push_back(const T& value) {
    if (count >= capacity) {
        resize(capacity * 2);
    }
    std::allocator_traits<Alloc>::construct(allocator, &data[count], value);
    ++count;
}

template <class T, class Alloc>
void PmrDynamicArray<T, Alloc>::pop_back() {
    assert(count > 0);
    std::allocator_traits<Alloc>::destroy(allocator, &data[count - 1]);
    --count;
}

template <class T, class Alloc>
T& PmrDynamicArray<T, Alloc>::operator[](size_t index) {
    assert(index < count);
    return data[index];
}

template <class T, class Alloc>
const T& PmrDynamicArray<T, Alloc>::operator[](size_t index) const {
    assert(index < count);
    return data[index];
}

template <class T, class Alloc>
T& PmrDynamicArray<T, Alloc>::front() { 
    assert(count > 0);
    return data[0]; 
}

template <class T, class Alloc>
T& PmrDynamicArray<T, Alloc>::back() { 
    assert(count > 0);
    return data[count - 1];
}

template <class T, class Alloc>
typename PmrDynamicArray<T, Alloc>::iterator PmrDynamicArray<T, Alloc>::begin() {
    return iterator(data);
}

template <class T, class Alloc>
typename PmrDynamicArray<T, Alloc>::iterator PmrDynamicArray<T, Alloc>::end() {
    return iterator(data + count);
}

template class PmrDynamicArray<int>;
template class PmrDynamicArray<Example>;