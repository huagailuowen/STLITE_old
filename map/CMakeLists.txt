cmake_minimum_required(VERSION 3.22)
project(map)

set(CMAKE_CXX_STANDARD 14)

include_directories(data)
include_directories(src)

add_executable(map
#        data/five/code.cpp
#        data/five.memcheck/code.cpp
#        data/four/code.cpp
#        data/four.memcheck/code.cpp
#        data/one/code.cpp
#        data/one.memcheck/code.cpp
#        data/three/code.cpp
#        data/three.memcheck/code.cpp
#        data/two/code.cpp
#        data/two.memcheck/code.cpp
#        data/class-bint.hpp
#        data/class-integer.hpp
#        data/class-matrix.hpp
        src/exceptions.hpp
        src/map.hpp
        src/utility.hpp
        code.cpp)
