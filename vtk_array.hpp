#pragma once

#include <string>
#include <iostream>
#include <fstream>

//abstract base class
class VTK_Array
{
    public:
        virtual ~VTK_Array() {};
    public:
        virtual size_t NoComponents() const = 0;
        virtual size_t NoEntities() const = 0;
        virtual void print() const = 0;
        virtual bool write(std::ofstream& OUTFILE) const = 0;
};

/*
VTK_DataArray(  
              std::string name_,              ->  name of the data array
              const T* data_start_,           ->  pointer to where data starts (must be of type double*, int* or sizte_t*)
              std::array<size_t, 2> shape_,   -> { number of entities e.g. 100 nodes, number of components e.g. 3 coordinates}
              std::array<size_t, 2> strides_  -> { strides between entities in bytes, strides between components in bytes}
              )
Very flexibel interface to the data.
Yet we assume a continuity of the data in memory as outlined below.
n -> NoEntities
m -> NoComponents
a -> stride between two components
b -> stride between two entities
[..... x11,  x12, x13, ..., x1m, ..... , xn1, xn2, xn3, ..., xnm, ...]
       |<-a->|                           |
       |<-------------- b -------------->|
Examples:
    E1: Nodal Coordinates flattened in a vector
        std::vector<double> XI({0.1, 1.0, 2.0, 10.0, 20.0, 30.0});
        VTK_DataArray("DoubleVectorExample", XI.data(), {2, 3}, {3*sizeof(XI[0]), sizeof(XI[0])});

    E2: Scalars in a Vector
        std::vector<int> scalars({1, 10, 2, 3, 11, 100});
        VTK_DataArray("IntVectorExample", scalars.data(), {scalars.size(), 1}, {sizeof(scalars[0]), sizeof(scalars[0])});
    
    E3: Pairs in a Vector-Array construct
        std::vector<std::array<size_t, 2>> uscalars({{1, 10}, {2, 3}, {11, 100}, {1000293, 2137774}});
        VTK_DataArray("SizetVectorExample", &uscalars[0][0], {uscalars.size(), uscalars[0].size()}, {sizeof(uscalars[0]), sizeof(uscalars[0][0])}).print();

    E4: More complex example where the data lies structured but not continuous in memory.
        We are interested in [value, value2] as a two component entity.
        1. Name is set.
        2. Starting pointer needs to point to the first value.
        3. The dimensions are length of the vector which contains the dummy objects times 2 for the two values.
        4. The stride between the value entries is exactly the size of dummy.
        The stride between value and value2 can be computed by hand: double value(8byte) + char name (1byte) = 9bytes
        ! Computation by hand might fail, due to padding. In these cases it is better to compute the distance at runtime!
        ! A possible solution is shown below.
        struct dummy
        {
        int index;
        double value;
        char name;
        double value2;
        };
        dummy a({1, -91.87, 'a', 51.43});
        size_t stride_between_values = ( (&a.value2)-(&a.value) ) * sizeof(a.value2);
        std::vector<dummy> DVec({{1, -9.87, 'a', 5.43}, {2, 8.76, 'b', -4.32}, {3, -7.65, 'c', 3.21}, {4, 6.54, 'd', -2.1}});
        VTK_DataArray("ObjectVectorExample", &DVec[0].value, {DVec.size(), 2}, {sizeof(DVec[0]), stride_between_values}).print();
*/
template <typename T>
class VTK_DataArray : public VTK_Array
{
    private:
        std::string DataArrayName;
        std::string DataType;
        size_t NoEntities_;
        size_t NoComponents_;
        const T* data;
        size_t strides_between_starts_of_entities; // in bits
        size_t strides_between_starts_of_components; // in bits
    public:
//      VTK_DataArray(  
//             std::string name_,              ->  name of the data array
//             const T* data_start_,           ->  pointer to where data starts (must be of type double*, int* or sizte_t*)
//             std::array<size_t, 2> shape_,   -> { number of entities e.g. 100 nodes, number of components e.g. 3 coordinates}
//             std::array<size_t, 2> strides_  -> { strides between entities in bytes, strides between components in bytes}
//             )
        VTK_DataArray(  
                        std::string name_,
                        const T* data_start_,
                        std::array<size_t, 2> shape_,
                        std::array<size_t, 2> strides_
                        )
        : DataArrayName(name_), NoEntities_(shape_[0]), NoComponents_(shape_[1]),
          data(data_start_), 
          strides_between_starts_of_entities(strides_[0]), 
          strides_between_starts_of_components(strides_[1]), 
          DataType(VTKType(data_start_[0])) 
        {}

    public:
        size_t NoComponents() const {return NoComponents_;}
        size_t NoEntities() const {return NoEntities_;}
    public:
        // Print the data to std::cout
        void print() const
        {
            std::cout << "NoEntities  : " << NoEntities_ << " strides_between_starts_of_entities  : " << strides_between_starts_of_entities << std::endl;
            std::cout << "NoComponents: " << NoComponents_ << " strides_between_starts_of_components: " << strides_between_starts_of_components << std::endl;
            std::cout << "VTKType   : " << DataType << std::endl;
            for (size_t ENTITY=0; ENTITY<NoEntities_; ENTITY++)
            {
                for (size_t COMP=0; COMP<NoComponents_; COMP++)
                {
                    char *ptr = (char*) data+ ENTITY*strides_between_starts_of_entities + COMP*strides_between_starts_of_components;
                    std::cout << *(T*) ptr << " ";
                }
                std::cout << std::endl;
            }
        }
    public:
        // Writes data to file
        bool write(std::ofstream& OUTFILE) const
        {   
            std::string format("ascii");
            // open data array
            OUTFILE << "<DataArray ";
            OUTFILE << "Name=\""                << DataArrayName << "\" ";
            OUTFILE << "NumberOfComponents=\""  << NoComponents_ << "\" ";
            OUTFILE << "format=\""              << format << "\" ";
            OUTFILE << "type=\""                << DataType << "\" ";
            OUTFILE << ">";
            for (size_t ENTITY=0; ENTITY<NoEntities_; ENTITY++)
                for (size_t COMP=0; COMP<NoComponents_; COMP++)
                {
                    char *ptr = (char*) data+ ENTITY*strides_between_starts_of_entities + COMP*strides_between_starts_of_components;
                    OUTFILE << *(T*) ptr<< " ";
                }
            // close data array
            OUTFILE << "</DataArray>" << std::endl;
            return true;
        }
        // Writes ELEMENT data to file ->! Skipps the NumberOfComponents tag
        bool ewrite(std::ofstream& OUTFILE) const
        {   
            std::string format("ascii");
            // open data array
            OUTFILE << "<DataArray ";
            OUTFILE << "Name=\""                << DataArrayName << "\" ";
            OUTFILE << "format=\""              << format << "\" ";
            OUTFILE << "type=\""                << DataType << "\" ";
            OUTFILE << ">";
            for (size_t ENTITY=0; ENTITY<NoEntities_; ENTITY++)
                for (size_t COMP=0; COMP<NoComponents_; COMP++)
                {
                    char *ptr = (char*) data+ ENTITY*strides_between_starts_of_entities + COMP*strides_between_starts_of_components;
                    OUTFILE << *(T*) ptr<< " ";
                }
            // close data array
            OUTFILE << "</DataArray>" << std::endl;
            return true;
        }
};