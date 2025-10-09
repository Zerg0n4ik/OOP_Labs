#ifndef OCTAL_H
#define OCTAL_H

#include <initializer_list>
#include <string>

class Octal {
private:
    unsigned char* digits;  
    size_t size;            
    
    void removeLeadingZeros();
    unsigned char charToDigit(char c) const;
    char digitToChar(unsigned char digit) const;
    bool isValidDigit(unsigned char digit) const;
    
public:
    Octal();
    Octal(const size_t& n, unsigned char value = 0);
    Octal(const std::initializer_list<unsigned char>& items);
    Octal(const std::string& value);
    
    Octal(const Octal& other);
    Octal(Octal&& other) noexcept;
    
    ~Octal() noexcept;
    
    Octal add(const Octal& other) const;
    Octal subtract(const Octal& other) const;
    
    bool equals(const Octal& other) const;
    bool lessThan(const Octal& other) const;
    bool greaterThan(const Octal& other) const;
    
    std::string toString() const;
};

#endif