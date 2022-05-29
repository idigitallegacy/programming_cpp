cmake_minimum_required(VERSION 3.16)
project(cpp)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

# LAB 2

add_executable(cpp lab2/main.cpp)

#enable_testing()
#add_executable(main_test test_cases.cc)
#target_link_libraries(main_test gtest_main gtest curl)
target_link_libraries(cpp curl jansson)

#include(GoogleTest)
#gtest_discover_tests(main_test)