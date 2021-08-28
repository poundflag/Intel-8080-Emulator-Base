cmake_minimum_required(VERSION 3.12) # version can be different

project(Intel-8080-Emulator) #name of your project

add_executable(main main.cpp) # add this executable

add_subdirectory(../test tests)