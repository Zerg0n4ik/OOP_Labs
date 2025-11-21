#include "./include/lab5.hpp"
#include <iostream>

int main() {
    FixedBlockMemoryResource memory(4096);
    std::pmr::polymorphic_allocator<int> intAlloc(&memory);
    std::pmr::polymorphic_allocator<Example> complexAlloc(&memory);

    std::cout << "Динамический массив с простыми типами (int):\n";
    PmrDynamicArray<int> array_int(intAlloc);

    array_int.push_back(1);
    array_int.push_back(2);
    array_int.push_back(3);
    array_int.push_back(4);
    array_int.push_back(5);
    array_int.push_back(6);

    std::cout << "Размер: " << array_int.size() << ", емкость: " << array_int.get_capacity() << "\n";
    std::cout << "Элементы: ";
    for (int z : array_int) 
        std::cout << z << " ";
    std::cout << "\n";

    array_int.pop_back();

    std::cout << "После pop_back(): ";
    for (int z : array_int) 
        std::cout << z << " ";
    std::cout << "\n";

    std::cout << "Динамический массив со сложными типами (struct с несколькими полями):\n";
    PmrDynamicArray<Example> array_struct(complexAlloc);

    array_struct.push_back(Example(1, 2));
    array_struct.push_back(Example(3, 4));
    array_struct.push_back(Example(5, 6));

    std::cout << "Элементы: ";
    for (const auto& w : array_struct)
        std::cout << "(" << w.x << ", " << w.y << ") ";
    std::cout << "\n";

    array_struct.pop_back();

    std::cout << "После pop_back(): ";
    for (const auto& w : array_struct)
        std::cout << "(" << w.x << ", " << w.y << ") ";
    std::cout << "\n";

    std::cout << "Доступ по индексу: array_struct[0] = (" << array_struct[0].x << ", " << array_struct[0].y << ")\n";

    return 0;
}