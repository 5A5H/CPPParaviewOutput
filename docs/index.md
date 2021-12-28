# The CppParaviewOutput library
The **CppParaviewOutput** library is a **C++** library that can be used to generate files to be read primarily by **Paraview**.
Although it is a **headder only library**, it is based on **CMake** for building tests and easy use in other projects.

## Disclaimer
This project covers only **very basic** stuff. The main motivation is to have an easy output option **for scientific codes**.
Currently, only **unstructured grids** in **ascii** **XML** format can be exportet.

## Concept and API
* **Easy to use**: This library attempts to be easy to be used in other projects with only a very basic api. 
  Exporting the data can be as simple as:
  ```
    VTK_UnstructuredGrid VTKOUT_1;
    VTKOUT.setPoints(...);
    VTKOUT.setElements(...);
    VTKOUT.addCellData(...);
    VTKOUT.addNodeData(...);
    VTKOUT.write("file.vtu");
  ```
* **Lightweight and fast**: The goal is to stay headder only to keep the library shallow. Also the data should not be copied,
  (at least not before exporting) for performance reasons and in order to be memory efficient.

* **Flexibility**: The functions for introducing the mesh data to be written to the file offer a very generic interface to be 
  adoptable for many different data structures of the host-code. Convinience interfaces might be added in the future, too.

## Install/ Use it in your project
See how to use/install at [Usage](usage.md).

## Requirements:
* `CMake > 3.1 `
* `C++17`