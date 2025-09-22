cmake_minimum_required(VERSION 3.5)
project( bpOpenCV )

set(CMAKE_CXX_CPPCHECK cppcheck --enable=all -inconclusive --force --inline-suppr --std=c++17 --suppress=missingIncludeSystem)
add_compile_options(-Wall -Wextra)
set(CMAKE_VERBOSE_MAKEFILE ON)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( bpOpenCV main.cpp )
target_link_libraries( bpOpenCV ${OpenCV_LIBS} )
