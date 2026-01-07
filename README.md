# POETConverter

## Overview

POETConverter is a tool for converting simulation data from the POET (Potsdam
Reactive Transport) simulator into the XDMF (eXtensible Data Model and Format)
format, suitable for visualization in ParaView and other scientific
visualization tools. The application processes time-series simulation data and
generates XDMF metadata files that reference HDF5 data files.

## Implementation Status

The project is currently transitioning from a Julia prototype to a C++
implementation.

- **C++ Version**: The primary, performant implementation using HighFive (HDF5),
  TinyXML2, and RInside.
- **Julia Prototype**: Located in `julia/xdmf.jl`, serves as the reference
  implementation.

## Requirements

### System Core
- **CMake** >= 3.20
- **C++23 compatible compiler** (e.g., GCC 14+, Clang 16+)
- **HDF5** (Development headers and libraries)
- **R Environment** (with `qs2`, `RInside` and `Rcpp` package installed)

### External Libraries (Automatically managed via CMake FetchContent)
- **HighFive**: Modern HDF5 C++ wrapper
- **TinyXML2**: Lightweight XML generator
- **CLI11**: Command-line argument parser

## Compilation Instructions

Clone the repository and navigate to the project directory:

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

You will find the `poet2paraview` executable in the `build` directory after a
successful build.

## Usage

### Command Line Interface

```bash
./poet2paraview <input_dir> <rows> <cols>
```

| Argument    | Description                              |
| :---------- | :--------------------------------------- |
| `input_dir` | Directory containing `iter_*.qs2` files  |
| `rows`      | Number of rows in the simulation grid    |
| `cols`      | Number of columns in the simulation grid |

### Example

The following command converts simulation data located in `data/simulation_01`
with a grid size of 400x400:

```bash
./poet2paraview data/simulation_01 400 400
```

This will generate:
- `simulation_01.h5`: The raw data in HDF5 format.
- `simulation_01.xdmf`: The metadata file to be opened in ParaView.

## Visualization with ParaView

1. Launch **ParaView**.
2. Go to `File -> Open` and select the `.xdmf` file.
3. Choose the **XDMF Reader** if prompted.
4. Click **Apply**.
5. Use the time controls to navigate through the simulation iterations.

