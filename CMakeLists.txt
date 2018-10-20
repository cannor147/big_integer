cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
project(big_integer)

set(GTests
        gtest/gtest_main.cc
        gtest/gtest-all.cc
        gtest/gtest.h
        )

set(BigInteger
        big_integer.cpp
        big_integer.h
        )
set(MyVector
        my_vector.cpp
        my_vector.h
        )

include_directories(${BIGINT_SOURCE_DIR})
add_executable(main main.cpp ${BigInteger} ${MyVector})
add_executable(big_integer_testing big_integer_testing.cpp ${GTests} ${BigInteger} ${MyVector})

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif()

target_link_libraries(big_integer_testing -lpthread)