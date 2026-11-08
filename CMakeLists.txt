# CMake build for any operating system (Windows, Linux, macOS).
#
#   cmake -S . -B build                 (Windows with MinGW: add  -G "MinGW Makefiles")
#   cmake --build build
#
# The program is written to output/main (output\main.exe on Windows), the same place
# the Makefile and the VS Code build use. Run it from inside output/.

cmake_minimum_required(VERSION 3.16)
project(StudentGrades VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Speed is measured, so build an optimized "Release" version unless told otherwise.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

add_executable(main
    main.cpp
    Student.cpp
    StudentIO.cpp
    Processing.cpp
)

if(MSVC)
    target_compile_options(main PRIVATE /W4)
else()
    # -O2 like the Makefile and the VS Code build, so all timings are comparable.
    target_compile_options(main PRIVATE -Wall -Wextra $<$<CONFIG:Release>:-O2>)
endif()

# Put the program in output/. The $<1:...> stops Visual Studio from adding a
# Release/ or Debug/ subfolder.
set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "$<1:${CMAKE_SOURCE_DIR}/output>")
