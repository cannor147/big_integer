cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
project(big_integer)

set(Sources main.cpp big_integer.cpp my_vector.cpp)
set(Headers big_integer.h my_vector.h)

add_executable(HW02 ${Sources} ${Headers})
include_directories(${BIGINT_SOURCE_DIR})

add_executable(big_integer_testing
        big_integer_testing.cpp
        big_integer.h
        big_integer.cpp
        my_vector.h
        my_vector.cpp
        gtest/gtest-all.cc
        gtest/gtest.h
        gtest/gtest_main.cc)

add_executable(big_integer
        big_integer.h
        big_integer.cpp
        my_vector.h
        my_vector.cpp
        main.cpp)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif()

target_link_libraries(big_integer_testing -lpthread)
