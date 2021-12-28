cmake_minimum_required(VERSION 3.1)
project(CPPParaviewOutput)

set (CMAKE_CXX_STANDARD 17)

include(CTest)

add_library(CPPParaviewOutput INTERFACE)
target_sources(CPPParaviewOutput INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vtk_array.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vtk_definitions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vtk_unstructuredgrid.hpp)
target_include_directories(CPPParaviewOutput INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(tests)