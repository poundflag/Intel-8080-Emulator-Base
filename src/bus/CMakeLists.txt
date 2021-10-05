add_library(buscontroller buscontroller.cpp)

add_library(ram
            device/ram.h
            device/ram.cpp
)

add_library(ramdebug
            device/ramdebug.h
            device/ramdebug.cpp
)

add_library(busdevice
            model/busdevice.h
            model/busdevice.cpp
)

add_library(chipregion
            model/chipregion.h
            model/chipregion.cpp
)

target_link_libraries(
    buscontroller
    PUBLIC ram
    PUBLIC ramdebug
    PUBLIC busdevice
    PUBLIC chipregion
)

target_include_directories(
    buscontroller
    PUBLIC "{CMAKE_SOURCE_DIR}/device"
    PUBLIC "{CMAKE_SOURCE_DIR}/model"
)