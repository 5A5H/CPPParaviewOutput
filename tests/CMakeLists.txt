# Test directory
if(BUILD_TESTING)

    # Test the vtk data array
    add_executable(vtkdataarraytest vtk_dataarray_test.cpp)
    target_include_directories(vtkdataarraytest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
    add_test(vtkdataarraytest vtkdataarraytest)

    # Test the vtu export
    add_executable(vtuexporttest vtk_unstructuredgrid_test.cpp)
    target_include_directories(vtuexporttest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
    add_test(vtuexporttest vtkdataarraytest)
    
endif(BUILD_TESTING)