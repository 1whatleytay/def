add_library(parser STATIC
    include/parser/return.h
    include/parser/assign.h
    include/parser/new.h
    include/parser/call.h
    include/parser/dot.h
    include/parser/parentheses.h
    include/parser/reference.h
    include/parser/number.h
    include/parser/type.h
    include/parser/operator.h
    include/parser/expression.h
    include/parser/variable.h
    include/parser/function.h
    include/parser/root.h
    include/parser/kinds.h

    src/root.cpp
    src/function.cpp
    src/variable.cpp
    src/expression.cpp
    src/operator.cpp
    src/type.cpp
    src/number.cpp
    src/reference.cpp
    src/parentheses.cpp
    src/dot.cpp
    src/call.cpp
    src/new.cpp
    src/assign.cpp
    src/return.cpp)

target_include_directories(parser PUBLIC include)
target_link_libraries(parser PUBLIC hermes)
