cmake_minimum_required(VERSION 3.12) # version can be different

project(Intel-8080-Emulator) #name of your project

set (CMAKE_CXX_STANDARD 11)

add_subdirectory(../lib/googletest googletest)

include_directories(../lib/googletest/googletest/include) # this is so we can #include <gtest/gtest.h>

include_directories(../src)

add_library(ram ../src/bus/device/ram.cpp)

add_library(buscontroller ../src/bus/buscontroller.cpp)

add_library(busdevice ../src/bus/model/busdevice.cpp)

add_library(chipregion ../src/bus/model/chipregion.cpp)

add_executable(tests main.cpp) # add this add_executable

target_link_libraries(tests PUBLIC ram buscontroller chipregion busdevice PRIVATE gtest) # link google test to this executable
