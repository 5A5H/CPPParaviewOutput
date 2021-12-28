#include <iostream>
#include <array>
#include <vector>

#include <vtk_array.hpp>

int main()
{
    std::vector<double> XI({0.1, 1.0, 2.0, 10.0, 20.0, 30.0});
    VTK_DataArray("DoubleVectorExample", XI.data(), {2, 3}, {3*sizeof(XI[0]), sizeof(XI[0])}).print();
    
    std::vector<int> scalars({1, 10, -2, 3, -11, 100});
    VTK_DataArray("IntVectorExample", scalars.data(), {scalars.size(), 1}, {sizeof(scalars[0]), sizeof(scalars[0])}).print();
    
    std::vector<std::array<size_t, 2>> uscalars({{1, 10}, {2, 3}, {11, 100}, {1000293, 2137774}});
    VTK_DataArray("SizetVectorExample", &uscalars[0][0], {uscalars.size(), uscalars[0].size()}, {sizeof(uscalars[0]), sizeof(uscalars[0][0])}).print();

    // More complex example where the data lies structured but not continuous in memory.
    // We are interested in [value, value2] as a two component entity.
    // 1. Name is set.
    // 2. Starting pointer needs to point to the first value.
    // 3. The dimensions are length of the vector which contains the dummy objects times 2 for the two values.
    // 4. The stride between the value entries is exactly the size of dummy.
    //    The stride between value and value2 can be computed by hand: double value(8byte) + char name (1byte) = 9bytes
    //    ! Computation by hand might fail, due to padding. In these cases it is better to compute the distance at runtime!
    //    ! A possible solution is shown below.
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

}