#include <iostream>
#include <stdexcept>

using namespace std;

class Octal {
    private:
        unsigned char* digits;
        size_t len;
    public:

    Octal() {
        len = 1;
        digits = new unsigned char[1];
        digits[0] = 0;
    }

    Octal(const string& str) {
    len = str.size();
    digits = new unsigned char[len];
    for (size_t i = 0; i < len; i++) {
        char c = str[len - 1 - i];
        if (c < '0' || c > '7') {
            throw invalid_argument("Некорректная цифра для восьмеричного числа");
        }
        digits[i] = c - '0';
        }
    }

    Octal(const Octal& other) {
        digits = new unsigned char [other.len];
        for (size_t i = 0; i < len - 1; i++) {
            digits[i] = other.digits[i];
        }
    }

    Octal add(const Octal& other) {
        size_t newLen = max(len, other.len) + 1;
        unsigned char* newDigits = new unsigned char [newLen];
        for (int i = 0; i < newLen; i++) {
            newDigits[i] = 0;
        }

        unsigned char past;
        unsigned char a;
        unsigned char b;
        unsigned char sum;
        for (size_t i = 0; i < newLen; i++) {
            if (i < len) {
                a = digits[i];
            } else {
                a = 0;
            }
            if (i < other.len) {
                b = other.digits[i];
            } else {
                b = 0;
            }

            sum = a + b + past;
            past = sum / 8;
        }

        newDigits[newLen] = past;

        size_t realLen = newLen;
        if (newDigits[newLen - 1] == 0) {
        realLen--; 
        }

        Octal result;
        result.len = realLen;
        result.digits = new unsigned char[realLen];
        for (size_t i = 0; i < realLen; i++) {
            result.digits[i] = newDigits[i];
        }

        delete[] newDigits;
        return result;
    }

    ~Octal() {
        delete[] digits;
    }

    bool isEqual(const Octal& other) const {
        if (len == other.len) {
            for (size_t i = len; i-- > 0;) {
                if (digits[i] != other.digits[i]) {
                    return false;
                }
            }
            return true;
        } else {
            return false;
        }
    }

    bool isBigger (const Octal& other) const {
        if (len > other.len) {
            return true;
        } else if (len < other.len) {
            return false;
        } else if (len == other.len) {
            for (size_t i = len; i-- > 0;) {
                if (digits[i] > other.digits[i]) {
                    return true;
                } else if (digits[i] == other.digits[i]) {
                    continue;
                } else {
                    return false;
                }
            }
        }
        return false;
    }

    bool isSmaller (const Octal& other) const {
        if (len < other.len) {
            return true;
        } else if (len > other.len) {
            return false;
        } else if (len == other.len) {
            for (size_t i = len; i-- > 0;) {
                if (digits[i] > other.digits[i]) {
                    return false;
                } else if (digits[i] == other.digits[i]) {
                    continue;
                } else {
                    return true;
                }
            }
        }
        return false;
    }
};
