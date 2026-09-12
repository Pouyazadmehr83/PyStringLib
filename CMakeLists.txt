cmake_minimum_required(VERSION 3.14)
project(PyStringLib 
    VERSION 2.0.0 
    DESCRIPTION "Pythonic string manipulation for Modern C++" 
    HOMEPAGE_URL "https://github.com/Pouyazadmehr83/PyStringLib"
    LANGUAGES CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(PYSTRING_BUILD_TESTS "Build test suite" ON)
option(PYSTRING_BUILD_EXAMPLES "Build examples" ON)
option(PYSTRING_BUILD_BENCHMARKS "Build benchmarks" ON)

# -----------------------------------------------------------------------------
# Core Library Target (Header-only INTERFACE)
# -----------------------------------------------------------------------------
add_library(pystring INTERFACE)
add_library(PyStringLib::pystring ALIAS pystring)

target_include_directories(pystring INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/single_include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(pystring INTERFACE cxx_std_17)

# -----------------------------------------------------------------------------
# Optional Compiled Target (Static / Shared)
# -----------------------------------------------------------------------------
add_library(pystring_static STATIC src/PyString.cpp)
target_link_libraries(pystring_static PUBLIC pystring)
set_target_properties(pystring_static PROPERTIES OUTPUT_NAME "pystring")

# -----------------------------------------------------------------------------
# Tests
# -----------------------------------------------------------------------------
if(PYSTRING_BUILD_TESTS)
    enable_testing()
    add_executable(test_all tests/test_all.cpp)
    target_link_libraries(test_all PRIVATE pystring)
    add_test(NAME PyStringLibTests COMMAND test_all)
endif()

# -----------------------------------------------------------------------------
# Examples
# -----------------------------------------------------------------------------
if(PYSTRING_BUILD_EXAMPLES)
    add_executable(example_basic examples/basic_usage.cpp)
    target_link_libraries(example_basic PRIVATE pystring)

    add_executable(example_slicing examples/slicing_demo.cpp)
    target_link_libraries(example_slicing PRIVATE pystring)

    add_executable(example_chaining examples/chaining_demo.cpp)
    target_link_libraries(example_chaining PRIVATE pystring)

    add_executable(pystring_repl tools/pystring_repl.cpp)
    target_link_libraries(pystring_repl PRIVATE pystring)
endif()

# -----------------------------------------------------------------------------
# Benchmarks
# -----------------------------------------------------------------------------
if(PYSTRING_BUILD_BENCHMARKS)
    add_executable(benchmark examples/benchmark.cpp)
    target_link_libraries(benchmark PRIVATE pystring)
endif()

# -----------------------------------------------------------------------------
# Installation & CMake Export
# -----------------------------------------------------------------------------
install(TARGETS pystring pystring_static
    EXPORT PyStringLibTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/pystring
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES include/PyString.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES single_include/pystring.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pystring
)

install(EXPORT PyStringLibTargets
    FILE PyStringLibTargets.cmake
    NAMESPACE PyStringLib::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PyStringLib
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/PyStringLibConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/PyStringLibConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/PyStringLibConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PyStringLib
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/PyStringLibConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/PyStringLibConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PyStringLib
)
