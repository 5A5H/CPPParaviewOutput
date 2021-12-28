# Test directory
if(BUILD_TESTING)

    # Test the vtk data array
    add_executable(vtkdataarraytest vtk_dataarray_test.cpp)
    target_include_directories(vtkdataarraytest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
    add_test(vtkdataarraytest vtkdataarraytest)
    
endif(BUILD_TESTING)