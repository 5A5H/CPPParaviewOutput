cmake_minimum_required(VERSION 3.1)
project(CPPParaviewOutput VERSION 1.0)

set (CMAKE_CXX_STANDARD 17)

include(CTest)

add_library(CPPParaviewOutput INTERFACE)
target_sources(CPPParaviewOutput INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vtk_array.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vtk_definitions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vtk_unstructuredgrid.hpp)
target_include_directories(CPPParaviewOutput INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

export(TARGETS CPPParaviewOutput FILE CPPParaviewOutputConfig.cmake)

add_subdirectory(tests)