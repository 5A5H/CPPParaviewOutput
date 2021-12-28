## Usage Screnarios:
### Bare include
You may use the headders in your project as they where self made:
```
....
#include ".../vtk_unstructuredgrid.hpp"
....
```

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

### CMake use as installed
Suppose you have downloaded and build (using CMake) this project `some/where/CPPParaviewOutput/build`.
Then you may use in the host project:
```
set(CPPParaviewOutput_DIR some/where/CPPParaviewOutput/build)
find_package(CPPParaviewOutput)
```
If the build is visible to CMake, the `find_package(CPPParaviewOutput)` might be sufficient.
Still your host-code's sources need to include the headders via
```
....
#include <vtk_unstructuredgrid.hpp>
....
```