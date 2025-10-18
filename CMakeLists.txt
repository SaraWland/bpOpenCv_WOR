cmake_minimum_required(VERSION 3.5)
project( bpOpenCV )

set(CMAKE_CXX_CPPCHECK cppcheck --enable=all -inconclusive --force --inline-suppr --std=c++17 --suppress=missingIncludeSystem)
add_compile_options(-Wall -Wextra)
set(CMAKE_VERBOSE_MAKEFILE ON)

find_program(VALGRIND_EXECUTABLE valgrind)
if(VALGRIND_EXECUTABLE)
    message(STATUS "Valgrind found: ${VALGRIND_EXECUTABLE}")
    set(VALGRIND_OPTIONS --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose)
    add_custom_target(valgrind
        COMMAND ${VALGRIND_EXECUTABLE} ${VALGRIND_OPTIONS} $<TARGET_FILE:bpOpenCV>
        DEPENDS bpOpenCV
        COMMENT "Running Valgrind memory check"
    )
else()
    message(WARNING "Valgrind not found. Memory leak detection will be unavailable.")
endif()

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( bpOpenCV main.cpp 
                openCvHandler.cpp
                circle.cpp 
                halfCircle.cpp
                triangle.cpp
                rectangle.cpp
                square.cpp
                shapeHandler.cpp
                colorManager.cpp
                inputHandler.cpp
                Logger.cpp
                parser.cpp )
target_link_libraries( bpOpenCV ${OpenCV_LIBS} )
