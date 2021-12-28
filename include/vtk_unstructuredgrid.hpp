#pragma once

#include <vtk_array.hpp>

class VTK_UnstructuredGrid
{
    public: 
        VTK_UnstructuredGrid() 
        : dimensions_(3), points_set_(false), cells_set_(false), NoPoints_(0), NoCells_(0),
        PointCoordinates_(nullptr), ElementConnectivity_(nullptr), CellData_(0), NodeData_(0)
        {}
        ~VTK_UnstructuredGrid()
        {
            delete PointCoordinates_;
            delete ElementConnectivity_;
            for (auto &data : CellData_) delete data;
            for (auto &data : NodeData_) delete data;
        }
    private:
        VTK_DataArray<double>* PointCoordinates_;
        VTK_DataArray<size_t>* ElementConnectivity_;
        std::vector<VTK_Array*> CellData_;
        std::vector<VTK_Array*> NodeData_;
    public:
        // Set points via VTK_DataArray interface
        bool setPoints(const double* data_start_, std::array<size_t, 2> shape_, std::array<size_t, 2> strides_);

        // convenience overload for n-nodal coordinates inside a vector XI = {x1, y1, z1, ...,xn, yn, zn}
        bool setPoints(const std::vector<double> &XI) { return setPoints(XI.data(), {XI.size()/3, 3}, {sizeof(double)*3, sizeof(double)});}

        // Set cells via VTK_DataArray interface
        bool setElements(const size_t* data_start_, std::array<size_t, 2> shape_, std::array<size_t, 2> strides_, VTK_CELLTYPE vtkcelltype);

        // convinience overload for elements in flattened format
        bool setElements(const std::vector<size_t> &Elmt, VTK_CELLTYPE vtkcelltype) { return setElements(Elmt.data(), {Elmt.size()/VTK_CELL_NODES(vtkcelltype), VTK_CELL_NODES(vtkcelltype)}, {sizeof(size_t)*VTK_CELL_NODES(vtkcelltype), sizeof(size_t)}, vtkcelltype); }
        
        // Set cell data
        template <typename T>
        bool addCellData(const std::string name, const T* data_start_, std::array<size_t, 2> shape_, std::array<size_t, 2> strides_);

        // convenience overload for m-component element data in a flattened format data_vector = {d11, d12, ..., d1m, d21, ..., dem} (e=number of elements)
        template <typename T>
        bool addCellData(const std::string name, const std::vector<T> &data_vector, const size_t no_components) { return addCellData(name, data_vector.data(), {NoCells_, no_components}, {sizeof(T)*no_components, sizeof(T)}); }

        // Set node data
        template <typename T>
        bool addNodeData(const std::string name, const T* data_start_, std::array<size_t, 2> shape_, std::array<size_t, 2> strides_);
        
        // convenience overload for m-component noda data in a flattened format data_vector = {d11, d12, ..., d1m, d21, ..., dnm} (n=number of nodes)
        template <typename T>
        bool addNodeData(const std::string name, const std::vector<T> &data_vector, const size_t no_components) { return addNodeData(name, data_vector.data(), {NoPoints_, no_components}, {sizeof(T)*no_components, sizeof(T)}); }

    public: 
        bool write(std::string path_to_file);
    private:
        unsigned int dimensions_;
        bool points_set_, cells_set_;
        size_t NoPoints_, NoCells_;
        VTK_CELLTYPE vtk_cell_type_;
};

bool VTK_UnstructuredGrid::write(std::string path_to_file)
{
    size_t nodim   = 3;
    size_t elmtnodes = VTK_CELL_NODES(vtk_cell_type_);
    std::string byte_order = "LittleEndian";
    std::ofstream outfile;
    outfile.open(path_to_file, std::ios::out);
    outfile << "<?xml version=\"1.0\" ?>" << std::endl;
    outfile << "<VTKFile byte_order=\""<< byte_order <<"\" type=\"UnstructuredGrid\" version=\"0.1\">" << std::endl;
    outfile << "<UnstructuredGrid>" << std::endl;
    outfile << "<Piece NumberOfCells=\"" << NoCells_ << "\" NumberOfPoints=\"" << NoPoints_ << "\">" << std::endl;
    outfile << "<Points>" << std::endl;
    PointCoordinates_ -> write(outfile);
    outfile << "</Points>" << std::endl;
    outfile << "<Cells>" <<std::endl;
    ElementConnectivity_ -> ewrite(outfile);
    outfile << "<DataArray Name=\"offsets\" format=\"ascii\" type=\"Int32\">";
    for (size_t i=0; i<NoCells_; i++) outfile << (i+1)*elmtnodes << "  ";
    outfile << "</DataArray>" << std::endl;
    outfile << "<DataArray Name=\"types\" format=\"ascii\" type=\"UInt8\">";
    for (size_t i=0; i<NoCells_; i++) outfile << vtk_cell_type_ << "  ";
    outfile << "</DataArray>" << std::endl;
    outfile << "</Cells>" <<std::endl;
    outfile << "<PointData>" <<std::endl;
    for (auto &data : NodeData_) data -> write(outfile);
    outfile << "</PointData>" <<std::endl;
    outfile << "<CellData>" <<std::endl;
    for (auto &data : CellData_) data -> write(outfile);
    outfile << "</CellData>" <<std::endl;
    outfile << "</Piece>" <<std::endl;
    outfile << "</UnstructuredGrid>" <<std::endl;
    outfile << "</VTKFile>" <<std::endl;
    return true;
}

bool VTK_UnstructuredGrid::setPoints(const double* data_start_, std::array<size_t, 2> shape_, std::array<size_t, 2> strides_)
{
    // setting points is always the first call:
    points_set_ = false;
    delete PointCoordinates_;
    cells_set_ = false;

    // check for valid input
    if (shape_[1] != 3) throw std::runtime_error("Error reading Point data! Wrong dimensions. Check your components input, got: "+std::to_string(shape_[1]));

    // compute number of points
    NoPoints_ = shape_[0];

    PointCoordinates_ = new VTK_DataArray("Coordinates", data_start_, shape_, strides_);

    if (PointCoordinates_!=nullptr)
    {
        points_set_ = true;
        return true;
    }
    return false;
}

bool VTK_UnstructuredGrid::setElements(const size_t* data_start_, std::array<size_t, 2> shape_, std::array<size_t, 2> strides_, VTK_CELLTYPE vtkcelltype)
{
    // check if points have already been set
    if (points_set_ != true) throw std::runtime_error("Error reading Cells! Call setPoints(...) first");

    // check if number of components (which are nodes per element) equals the VTK_CELLTYPE
    if (shape_[1] != VTK_CELL_NODES(vtkcelltype)) throw std::runtime_error("Error reading Cells! Your cell type demands "+std::to_string(VTK_CELL_NODES(vtkcelltype))+" nodes, your input has "+std::to_string(shape_[1])+" nodes per element.");

    delete ElementConnectivity_;
    cells_set_ = false;
    vtk_cell_type_ = vtkcelltype;

    ElementConnectivity_ = new VTK_DataArray("connectivity", data_start_, shape_, strides_);
    NoCells_ = shape_[0];

    if (ElementConnectivity_!=nullptr)
    {
        cells_set_ = true;
        return true;
    }
    return false;
}

template <typename T>
bool VTK_UnstructuredGrid::addNodeData(const std::string name, const T* data_start_, std::array<size_t, 2> shape_, std::array<size_t, 2> strides_)
{
    // check if points have already been set
    if (points_set_ != true) throw std::runtime_error("Error reading Cells! Call setPoints(...) first");

    // check if node data meets number of nodes
    if (shape_[0] != NoPoints_)throw std::runtime_error("Error adding NodeData! Number of NodeData does not match number of present nodes");

    // create a vtk data array
    auto NewNodeData_ = new VTK_DataArray(name, data_start_, shape_, strides_);
    NodeData_.push_back(NewNodeData_);

    if (NewNodeData_!=nullptr) return true;
    return false;
}

template <typename T>
bool VTK_UnstructuredGrid::addCellData(const std::string name, const T* data_start_, std::array<size_t, 2> shape_, std::array<size_t, 2> strides_)
{
    // check if elements have already been set
    if (cells_set_ != true) throw std::runtime_error("Error adding CellData! Call setElements(...) first");

    // check if cell data meets number of elements
    if (shape_[0] != NoCells_)throw std::runtime_error("Error adding CellData! Number of CellData does not match number of present elements");

    // create a vtk data array
    auto NewCellData_ = new VTK_DataArray(name, data_start_, shape_, strides_);
    CellData_.push_back(NewCellData_);

    if (NewCellData_!=nullptr) return true;
    return false;
}