add_library(alu
            alu.h
            alu.cpp
)

add_library(instructions
            instructions.h
            instructions.cpp
)

target_link_libraries(
    alu
    instructions
)