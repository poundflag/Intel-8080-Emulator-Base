add_library(
    instructiondecoder
    instructiondecoder.h
    instructiondecoder.cpp
)

add_library(instructions
            instructions.h
            instructions.cpp
)

target_link_libraries(
    instructiondecoder
    PUBLIC instructions
)