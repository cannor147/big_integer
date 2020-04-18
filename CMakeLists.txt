cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
project(big_integer)

set(GTests
        src/gtest/gtest_main.cc
        src/gtest/gtest-all.cc
        src/gtest/gtest.h
        )

set(BigInteger
        src/big_integer.cpp
        src/big_integer.h
        )
set(MyVector
        src/my_vector.cpp
        src/my_vector.h
        )

include_directories(${BIGINT_SOURCE_DIR})
add_executable(main src/main.cpp ${BigInteger} ${MyVector})
add_executable(big_integer_testing src/big_integer_testing.cpp ${GTests} ${BigInteger} ${MyVector})

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif ()

target_link_libraries(big_integer_testing -lpthread)
