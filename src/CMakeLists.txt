cmake_minimum_required(VERSION 3.12) # version can be different

project(Intel-8080-Emulator) #name of your project

set(CMAKE_CXX_STANDARD 11)

add_executable(main main.cpp) # add this executable

add_library(flagRegister registers/model/flagregister.cpp)
add_library(instructions instructions.cpp)
add_library(memoryreference registers/model/memoryreference.cpp)

target_link_libraries(
    main
    registercontroller
    flagRegister
    instructions
    stack
    buscontroller
    chipregion
    register
    memoryreference
)

add_subdirectory(../test tests)