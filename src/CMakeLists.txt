add_subdirectory(parser)
add_subdirectory(interpreter)

add_executable(def main.cpp)
target_link_libraries(def PUBLIC parser interpreter fmt)
