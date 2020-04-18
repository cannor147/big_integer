#include <iostream>
#include "big_integer.h"

int main() {
    big_integer a("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    big_integer b(                                                     "100000000000000000000000000000000000000");
    a *= b;

    std::cout << a << std::endl;
    return 0;
}