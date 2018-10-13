#include <iostream>
#include <vector>
#include "big_integer.h"
#include "my_vector.h"

int main() {
    /*big_integer a("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    big_integer b("100000000000000000000000000000000000000");
    big_integer c("100000000000000000000000000000000000000000000000000000");*/
    big_integer a(20);
    big_integer b(5);
    a /= b;

    std::cout << a << std::endl;
    return 0;
}