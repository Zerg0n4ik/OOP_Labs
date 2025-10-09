#include "../include/lab2_utils.h"
#include <stdexcept>
#include <algorithm>

// Символ в число
unsigned char Octal::charToDigit(char c) const {
    if (c >= '0' && c <= '7') {
        return c - '0';
    }
    throw std::invalid_argument("Не восьмиричная");
}

// число в символ
char Octal::digitToChar(unsigned char digit) const {
    if (digit < 8) {
        return '0' + digit;
    }
    throw std::invalid_argument("Не восьмиричная");
}

// чек на 8ричную
bool Octal::isValidDigit(unsigned char digit) const {
    return digit < 8;
}

// Удаляем ненужные 0
void Octal::removeLeadingZeros() {
    while (size > 1 && digits[size - 1] == 0) {
        size--;
    }
    unsigned char* new_digits = new unsigned char[size];
    for (int i = 0; i < size; i++) {
        new_digits[i] = digits[i];
    }
    delete[] digits;
    digits = new_digits;
}    

// Создаем 0
Octal::Octal() : size(1) {
    digits = new unsigned char[1];
    digits[0] = 0;
}

// Заполнение одним и тем же
Octal::Octal(const size_t& n, unsigned char value) : size(n) {
    if (!isValidDigit(value)) {
        throw std::invalid_argument("Число не в восьмиричной");
    }
    digits = new unsigned char[n];
    for (size_t i = 0; i < n; i++) {
        digits[i] = value;
    }
    removeLeadingZeros();
}

// Переворот
Octal::Octal(const std::initializer_list<unsigned char>& items) : size(items.size()) {
    digits = new unsigned char[size];
    size_t i = 0;
    for (auto it = items.end() - 1; it >= items.begin(); it--) {
        if (!isValidDigit(*it)) {
            throw std::invalid_argument("Число не в восьмиричной");
        }
        digits[i] = *it;
        i++;
    }
    removeLeadingZeros();
}

// Из строки в число
Octal::Octal(const std::string& value) : size(value.length()) {
    digits = new unsigned char[size];
    for (size_t i = 0; i < size; i++) {
        char c = value[size - 1 - i];
        digits[i] = charToDigit(c);
    }
    removeLeadingZeros();
}

// В строку из числа
std::string Octal::toString() const {
    std::string result;
    for (int i = size - 1; i >= 0; i--) {
        result += digitToChar(digits[i]);
    }
    return result;
}

// Копирование
Octal::Octal(const Octal& other) : size(other.size) {
    digits = new unsigned char[size];
    for (size_t i = 0; i < size; i++) {
        digits[i] = other.digits[i];
    }
}

// Копирование с удалением
Octal::Octal(Octal&& other) noexcept : digits(other.digits), size(other.size) {
    other.digits = nullptr;
    other.size = 0;
}

Octal::~Octal() noexcept {
    delete[] digits;
}

// Сумма
Octal Octal::add(const Octal& other) const {
    size_t new_size = std::max(size, other.size) + 1;
    unsigned char* new_digits = new unsigned char[new_size];
    unsigned char carry = 0;
    for (size_t i = 0; i < new_size; i++) {
        unsigned char sum = carry;
        if (i < size) {
            sum += digits[i];
        }
        if (i < other.size) {
            sum += other.digits[i];
        }
        new_digits[i] = sum % 8;
        carry = sum / 8;
    }
    Octal result;
    delete[] result.digits;
    result.digits = new_digits;
    result.size = new_size;
    result.removeLeadingZeros();
    return result;
}

// Разность
Octal Octal::subtract(const Octal& other) const {
    if (lessThan(other)) {
        throw std::invalid_argument("Без негатива");
    }
    unsigned char* new_digits = new unsigned char[size];
    unsigned char borrow = 0;
    for (size_t i = 0; i < size; i++) {
        int sub = digits[i] - borrow;
        if (i < other.size) sub -= other.digits[i];
        if (sub < 0) {
            sub += 8;
            borrow = 1;
        } else {
            borrow = 0;
        }
        new_digits[i] = sub;
    }
    Octal result;
    delete[] result.digits;
    result.digits = new_digits;
    result.size = size;
    result.removeLeadingZeros();
    return result;
}

// Сравнения
bool Octal::equals(const Octal& other) const {
    if (size != other.size) return false;
    for (size_t i = 0; i < size; ++i) {
        if (digits[i] != other.digits[i]) {
            return false;
        }
    }
    return true;
}

bool Octal::lessThan(const Octal& other) const {
    if (size != other.size) return size < other.size;
    for (int i = size - 1; i >= 0; --i) {
        if (digits[i] != other.digits[i]) {
            return digits[i] < other.digits[i];
        }
    }
    return false;
}

bool Octal::greaterThan(const Octal& other) const {
    return !lessThan(other) && !equals(other);
}