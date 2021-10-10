add_library(iocontroller iocontroller.cpp)

add_library(
    iodevice
    model/iodevice.cpp
    model/iodevice.h
)

add_library(
    serialout
    device/serialout.cpp
    device/serialout.h
)

target_link_libraries(
    iocontroller
    PUBLIC iodevice
    PUBLIC serialout
)

target_include_directories(
    iocontroller
    PUBLIC "{CMAKE_SOURCE_DIR}/model"
    PUBLIC "{CMAKE_SOURCE_DIR}/device"
    PUBLIC "{CMAKE_SOURCE_DIR}/"
)