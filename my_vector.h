#ifndef BIG_INTEGER_MY_VECTOR_H
#define BIG_INTEGER_MY_VECTOR_H

#include <vector>
#include <stdint-gcc.h>
#include <memory>

const size_t SMALL_SIZE = 32;

class my_vector final {

    typedef int32_t word_t;

private:
    struct object_t final {
        std::array<word_t, SMALL_SIZE> small;
        std::shared_ptr<std::vector<word_t>> big;

        object_t() {
            small = std::array<word_t, SMALL_SIZE>();
            big = std::make_shared<std::vector<word_t>>(0);
        }

        object_t& operator=(const object_t &p) {
            this->small = p.small;
            this->big = p.big;
            return *this;
        }

        word_t* data(bool f) {
            if (f) {
                return (&small[0]);
            } else {
                return &(big.get()->operator[](0));
            }
        }

        const word_t* data(bool f) const {
            if (f) {
                return (&small[0]);
            } else {
                return &(big.get()->operator[](0));
            }
        }
    };

    size_t n;
    object_t o;
    bool isSmall;

    void small_to_big();
    void big_to_small();
    void __reset();

public:
    my_vector();
    explicit my_vector(size_t, word_t);
    my_vector(my_vector const &);
    ~my_vector();

    my_vector& operator=(my_vector const &);

    void clear();
    bool empty() const;
    size_t size() const;

    word_t& operator[](size_t);
    word_t& back();
    const word_t& operator[](size_t) const;
    const word_t& back() const;
    void push_back(word_t);
    void pop_back();
    void fill(word_t);
    void resize(size_t);
    void resize(size_t, word_t);
};


#endif //BIG_INTEGER_MY_VECTOR_H