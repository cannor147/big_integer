#include <cassert>
#include "my_vector.h"

my_vector::my_vector() {
    n = 0;
    o = object_t();
    isSmall = true;
}
my_vector::my_vector(size_t s, word_t value = 0) {
    n = s;
    o = object_t();
    if (n <= SMALL_SIZE) {
        o.small.fill(value);
        o.big = std::make_shared<std::vector<word_t>>(0);
        isSmall = true;
    } else {
        o.big = std::make_shared<std::vector<word_t>>(n, value);
        isSmall = false;
    }
}
my_vector::my_vector(my_vector const &original) {
    n = original.n;
    o = original.o;
    isSmall = original.isSmall;
}
my_vector::~my_vector() {
    o.big.reset();
}

my_vector& my_vector::operator=(my_vector const &original) = default;

void my_vector::clear() {
    n = 0;
    o.big.reset();
    isSmall = true;
}
bool my_vector::empty() const {
    return (n == 0);
}
size_t my_vector::size() const {
    return n;
}

my_vector::word_t &my_vector::operator[](size_t index) {
    assert(n > 0 && index < n);
    __reset();
    return o.data(isSmall)[index];
}
my_vector::word_t &my_vector::back() {
    return operator[](n - 1);
}
const my_vector::word_t &my_vector::operator[](size_t index) const {
    assert(n > 0 && index < n);
    return o.data(isSmall)[index];
}
const my_vector::word_t &my_vector::back() const {
    return operator[](n - 1);
}
void my_vector::push_back(my_vector::word_t value) {
    __reset();
    if (isSmall) {
        if (n < SMALL_SIZE) {
            o.small[n] = value;
        } else {
            small_to_big();
            o.big.get()->push_back(value);
        }
    } else {
        o.big.get()->push_back(value);
    }
    n++;
}
void my_vector::pop_back() {
    assert(n > 0);
    __reset();
    if (!isSmall) {
        o.big.get()->pop_back();
        if (n + 1 == SMALL_SIZE) {
            big_to_small();
        }
    }
    n--;
}
void my_vector::fill(my_vector::word_t value) {
    assert(n > 0);
    __reset();
    std::fill(&o.data(isSmall)[0], &o.data(isSmall)[n - 1], value);
}
void my_vector::resize(size_t s) {
    assert(s > 0);
    n = s;
    if (n <= SMALL_SIZE && !isSmall) {
        big_to_small();
    }
    if (n > SMALL_SIZE && isSmall) {
        small_to_big();
    }
    if (n > SMALL_SIZE) {
        __reset();
    }
}
void my_vector::resize(size_t s, word_t value) {
    resize(s);
    fill(value);
}

void my_vector::small_to_big() {
    isSmall = false;
    o.big = std::make_shared<std::vector<word_t>>(n);
    for (size_t i = 0; i < SMALL_SIZE; i++) {
        o.big.get()->operator[](i) = o.small[i];
    }
}
void my_vector::big_to_small() {
    isSmall = true;
    for (size_t i = 0; i < SMALL_SIZE; i++) {
        o.small[i] = o.big.get()->operator[](i);
    }
    o.big.reset();
}

void my_vector::__reset() {
    if (!isSmall && !o.big.unique()) {
        o.big.reset(new std::vector<word_t>(*o.big.get()));
    }
}