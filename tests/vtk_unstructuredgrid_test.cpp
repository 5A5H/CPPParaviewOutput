#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>

#include <vtk_unstructuredgrid.hpp>

int main()
{
    std::cout << "Test SimpleParaviewExport" << std::endl;

    // Test 1:
    // Simple test mesh in 3D with four nodes (3 coordinates each)
    // and two elements of type VTK_TRIANGLE
    std::vector<double> XI_1({0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0});
    std::vector<size_t> Elmt_1({0, 1, 2, 2, 3, 0});
    std::vector<double> NodeData1({1, 0, 0, 0.5, 0.5, 0, 0, 1, 0, -0.2, 0.8, 0});
    std::vector<size_t> ElmtData1({22, 220});
    size_t nonodalcoords1 = 3; /*number of coordinates*/
    size_t nonodes1 = XI_1.size()/nonodalcoords1; /*number of nodes*/
    size_t nonodesperelmt1 = 3; /*number of nodes per element*/
    size_t nonelmts1 = Elmt_1.size()/nonodesperelmt1; /*number of elements*/

    // design API for VTK_UnstructuredGrid
    VTK_UnstructuredGrid VTKOUT_1;
    VTKOUT_1.setPoints(XI_1.data(), {nonodes1, nonodalcoords1}, {sizeof(double)*3, sizeof(double)});
    VTKOUT_1.setElements(Elmt_1.data(), {nonelmts1, nonodesperelmt1}, {sizeof(size_t)*3, sizeof(size_t)}, VTK_TRIANGLE);
    VTKOUT_1.addCellData("Mass", ElmtData1.data(), {nonelmts1, 1}, {sizeof(size_t), sizeof(size_t)});
    VTKOUT_1.addNodeData("Velocity", NodeData1.data(), {nonodes1, 3}, {sizeof(double)*3, sizeof(double)});
    VTKOUT_1.write("msh_t1.vtu");
    return 0;
}
