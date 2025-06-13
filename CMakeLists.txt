cmake_minimum_required(VERSION 3.14)
project(StudentMiniSMS)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(sms main.cpp)

enable_testing()
add_executable(tests tests.cpp)
add_test(NAME run_tests COMMAND tests)
