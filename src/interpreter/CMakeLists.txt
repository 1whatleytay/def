add_library(interpreter STATIC
    include/interpreter/error.h
    include/interpreter/interpreter.h

    src/interpreter.cpp
    src/error.cpp)

target_include_directories(interpreter PUBLIC include)
target_link_libraries(interpreter PUBLIC parser fmt)
