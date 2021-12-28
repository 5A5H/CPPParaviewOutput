# Test directory
if(BUILD_TESTING)

    # Test the vtk data array
    add_executable(vtkdataarraytest vtk_dataarray_test.cpp)
    target_link_libraries(vtkdataarraytest CPPParaviewOutput)
    add_test(vtkdataarraytest vtkdataarraytest)

    # Test the vtu export
    add_executable(vtuexporttest vtk_unstructuredgrid_test.cpp)
    target_link_libraries(vtuexporttest CPPParaviewOutput)
    add_test(vtuexporttest vtkdataarraytest)
    
endif(BUILD_TESTING)