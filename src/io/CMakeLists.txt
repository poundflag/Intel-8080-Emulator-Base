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

add_library(
    altairswitches
    device/altairswitches.cpp
    device/altairswitches.h
)

add_library(
    sio
    device/sio.cpp
    device/sio.h
)

target_link_libraries(
    iocontroller
    PUBLIC iodevice
    PUBLIC serialout
    PUBLIC altairswitches
    PUBLIC sio
)

target_include_directories(
    iocontroller
    PUBLIC "{CMAKE_SOURCE_DIR}/model"
    PUBLIC "{CMAKE_SOURCE_DIR}/device"
    PUBLIC "{CMAKE_SOURCE_DIR}/"
)