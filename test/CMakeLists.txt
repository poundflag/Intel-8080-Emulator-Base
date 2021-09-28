cmake_minimum_required(VERSION 3.14)

project(Intel-8080-Emulator) #name of your project

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  tests
  main.cpp
)

add_library(ram ../src/bus/device/ram.cpp)
add_library(busdevice ../src/bus/model/busdevice.cpp)
add_library(buscontroller ../src/bus/buscontroller.cpp)
add_library(chipregion ../src/bus/model/chipregion.cpp)
add_library(register ../src/registers/model/register.cpp)
add_library(flagregister ../src/registers/model/flagregister.cpp)
add_library(stack ../src/registers/model/stack.cpp)
add_library(registercontroller ../src/registers/registercontroller.cpp)
add_library(instructions1 ../src/instructions.cpp)

target_link_libraries(
  tests
  gtest_main
  ram
  busdevice
  buscontroller
  chipregion
  register
  flagregister
  stack
  registercontroller
  instructions1
  memoryreference
  ramdebug
)

include(GoogleTest)
gtest_discover_tests(tests)
