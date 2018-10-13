#ifndef HW02_BIG_INTEGER_H
#define HW02_BIG_INTEGER_H

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <bitset>
#include "my_vector.h"


//todo: for 16
typedef int32_t word_t;
typedef uint32_t uword_t;
typedef uint64_t udword_t;

const uword_t UWORD_MAX = UINT32_MAX;
const size_t CELL_SIZE = 32;

typedef std::vector<word_t> vec;
//typedef my_vector vec;

class big_integer {

private:
    vec number;

protected:

    bool check_for_zero() const;
    bool check_for_negative() const;
    bool check_for_positive() const;

    void resize(size_t size);
    void optimize();

    big_integer div_ms(big_integer &first, big_integer &second);
    big_integer add_ms(big_integer const &arg, size_t x, word_t y);
    big_integer subtract_ms(big_integer const &arg, size_t x, word_t y);

public:
    big_integer();
    big_integer(word_t value);
    explicit big_integer(std::string const &word);
    big_integer(big_integer const &original);
    ~big_integer();

    big_integer operator+() const;
    big_integer operator-() const;
    big_integer operator~() const;

    big_integer& operator++();
    big_integer& operator--();
    const big_integer operator++(int);
    const big_integer operator--(int);

    big_integer& operator=(big_integer const &arg);
    big_integer& operator+=(big_integer const &arg);
    big_integer& operator-=(big_integer const &arg);
    big_integer& operator*=(big_integer const &arg);
    big_integer& operator/=(big_integer const &arg);
    big_integer& operator%=(big_integer const &arg);
    big_integer& operator&=(big_integer const &arg);
    big_integer& operator|=(big_integer const &arg);
    big_integer& operator^=(big_integer const &arg);
    big_integer& operator>>=(word_t arg);
    big_integer& operator<<=(word_t arg);

    friend bool operator==(big_integer const &first, big_integer const &second);
    friend bool operator!=(big_integer const &first, big_integer const &second);
    friend bool operator<(big_integer const &first, big_integer const &second);
    friend bool operator>(big_integer const &first, big_integer const &second);
    friend bool operator<=(big_integer const &first, big_integer const &second);
    friend bool operator>=(big_integer const &first, big_integer const &second);

    friend std::string to_string(big_integer const &arg);
    friend word_t sign(big_integer const &first);
    friend big_integer abs(big_integer const &first);
    friend big_integer pow(big_integer const &first, word_t second);

    std::string to_binary() const;
    big_integer from_binary(std::vector<char> &v);
};

big_integer operator+(big_integer first, big_integer const &second);
big_integer operator-(big_integer first, big_integer const &second);
big_integer operator*(big_integer first, big_integer const &second);
big_integer operator/(big_integer first, big_integer const &second);
big_integer operator%(big_integer first, big_integer const &second);
big_integer operator&(big_integer first, big_integer const &second);
big_integer operator|(big_integer first, big_integer const &second);
big_integer operator^(big_integer first, big_integer const &second);
big_integer operator>>(big_integer first, word_t second);
big_integer operator<<(big_integer first, word_t second);

bool operator==(big_integer const &first, big_integer const &second);
bool operator!=(big_integer const &first, big_integer const &second);
bool operator<(big_integer const &first, big_integer const &second);
bool operator>(big_integer const &first, big_integer const &second);
bool operator<=(big_integer const &first, big_integer const &second);
bool operator>=(big_integer const &first, big_integer const &second);

std::istream& operator>>(std::istream& i, big_integer &some_bi);
std::ostream& operator<<(std::ostream& o, big_integer some_bi);

std::string to_string(big_integer const &arg);
word_t sign(big_integer const &first);
big_integer abs(big_integer const &first);
big_integer pow(big_integer const &first, word_t second);

#endif //HW02_BIG_INTEGER_H
