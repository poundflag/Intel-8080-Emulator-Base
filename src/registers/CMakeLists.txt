add_library(registercontroller registercontroller.cpp)

add_library(
    register1
    registers.cpp
    registers.h
)

add_library(
    flagregister
    model/flagregister.cpp
    model/flagregister.h
)

add_library(
    memoryreference
    model/memoryreference.cpp
    model/memoryreference.h
)

add_library(
    register
    model/register.cpp
    model/register.h
)

add_library(
    stack
    model/stack.cpp
    model/stack.h
)

target_link_libraries(
    registercontroller
    PUBLIC register1
    PUBLIC flagregister
    PUBLIC memoryreference
    PUBLIC register
    PUBLIC stack
)

target_include_directories(
    registercontroller
    PUBLIC "{CMAKE_SOURCE_DIR}/model"
    PUBLIC "{CMAKE_SOURCE_DIR}/"
)