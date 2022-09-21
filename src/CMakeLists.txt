cmake_minimum_required(VERSION 3.12) # version can be different

project(Intel-8080-Emulator) #name of your project

set(CMAKE_CXX_STANDARD 11)

add_executable(main main.cpp) # add this executable

add_subdirectory(bus)
add_subdirectory(registers)
add_subdirectory(../test tests)
add_subdirectory(io)
add_subdirectory(core)
add_subdirectory(instruction)

add_library(
    cpu
    cpu.cpp
    cpu.h
)

target_link_libraries(
    main
    PUBLIC buscontroller
    PUBLIC registercontroller
    PUBLIC iocontroller
    PUBLIC cpu
    PUBLIC instructions
    PUBLIC alu
    PUBLIC curses
    PUBLIC instructiondecoder
)

target_include_directories(
    main
    PUBLIC "{PROJECT_SOURCE_DIR}/src/"
    PUBLIC "{PROJECT_SOURCE_DIR}/src/bus"
    PUBLIC "{PROJECT_SOURCE_DIR}/src/registers"
    PUBLIC "{PROJECT_SOURCE_DIR}/src/io"
    PUBLIC "{PROJECT_SOURCE_DIR}/src/alu"
    PUBLIC "{PROJECT_SOURCE_DIR}/src/instruction"
)