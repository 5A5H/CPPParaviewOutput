#pragma once

#include <string>

namspace CPPP {};

/**!
 * \breif The VTK-CELLTYPE
 * Examples: 
          VTK_VERTEX (1 Nodes), 
          VTK_LINE (2 Nodes), 
          VTK_TRIANGLE (3 Nodes), 
          VTK_QUAD (4 Nodes), 
          VTK_TETRA (4 Nodes), 
          VTK_HEXAHEDRON (8 Nodes), 
          VTK_QUADRATIC_EDGE (3 Nodes),
          VTK_QUADRATIC_TRIANGLE (6 Nodes),
          VTK_QUADRATIC_QUAD (8 Nodes),
          VTK_QUADRATIC_TETRA (10 Nodes),
          VTK_QUADRATIC_HEXAHEDRON (20 Nodes)
*/
enum VTK_CELLTYPE
{
    VTK_VERTEX = 1,
    VTK_POLY_VERTEX = 2,
    VTK_LINE = 3,
    VTK_POLY_LINE = 4,
    VTK_TRIANGLE = 5,
    VTK_TRIANGLE_STRIP = 6,
    VTK_POLYGON = 7,
    VTK_PIXEL = 8,
    VTK_QUAD = 9,
    VTK_TETRA = 10,
    VTK_VOXEL = 11,
    VTK_HEXAHEDRON = 12,
    VTK_WEDGE = 13,
    VTK_PYRAMID = 14,
    VTK_QUADRATIC_EDGE = 21,
    VTK_QUADRATIC_TRIANGLE = 22,
    VTK_QUADRATIC_QUAD = 23,
    VTK_QUADRATIC_TETRA = 24,
    VTK_QUADRATIC_HEXAHEDRON = 25,
};

/*
Nodes per VTK_CELLTYPE 
Notice that all *POLY* types are not supported, 
if 0 zero is returned it must be handeled outside this function.
*/
inline unsigned int VTK_CELL_NODES(VTK_CELLTYPE type)
{
    switch (type)
    {
    case VTK_VERTEX:                return 1;
    case VTK_POLY_VERTEX:           return 0;
    case VTK_LINE:                  return 2;
    case VTK_POLY_LINE:             return 0;
    case VTK_TRIANGLE:              return 3;
    case VTK_TRIANGLE_STRIP:        return 0;
    case VTK_POLYGON:               return 0;
    case VTK_PIXEL:                 return 4;
    case VTK_QUAD:                  return 4;
    case VTK_TETRA:                 return 4;
    case VTK_VOXEL:                 return 8;
    case VTK_HEXAHEDRON:            return 8;
    case VTK_WEDGE:                 return 6;
    case VTK_PYRAMID:               return 5;
    case VTK_QUADRATIC_EDGE:        return 3;
    case VTK_QUADRATIC_TRIANGLE:    return 6;
    case VTK_QUADRATIC_QUAD:        return 8;
    case VTK_QUADRATIC_TETRA:       return 10;
    case VTK_QUADRATIC_HEXAHEDRON:  return 20;
    default: return 0;
    }
}

/*
Supports only types of data: 
UInt8, Int32, UInt64, Float64
*/
inline std::string VTKType(const size_t &)          {return "UInt32";}
inline std::string VTKType(const VTK_CELLTYPE &)    {return "UInt8";}
inline std::string VTKType(const double &)          {return "Float64";}
inline std::string VTKType(const int &)             {return "Int32";}