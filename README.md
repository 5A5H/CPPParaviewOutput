# CPPParaviewOutput
A very basic VTU-generator.

The documentation of this library can be found on [Project documentation](https://5a5h.github.io/CPPParaviewOutput/).

## Using:
### CMake in project usage
Add this directory to the host-project via (e.g. as a git-submodule):
```
add_subdirectory(submodules/CPPParaviewOutput)
```
Then use the library by including the headder in the source file:
```
....
#include <vtk_unstructuredgrid.hpp>
....
```
Finally on all sources which include the headders use:
```
target_link_libraries(host-project-target CPPParaviewOutput)
```