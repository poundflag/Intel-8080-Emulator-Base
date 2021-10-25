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

target_link_libraries(
  tests
  PUBLIC gtest_main
  PUBLIC buscontroller
  PUBLIC registercontroller
  PUBLIC iocontroller
  PUBLIC cpu
  PUBLIC instructions
  PUBLIC alu
)

include(GoogleTest)
gtest_discover_tests(tests)
