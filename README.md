# POETConverter

## Overview

POETConverter is a tool for converting simulation data from the POET (Potsdam
Reactive Transport) simulator into the XDMF (eXtensible Data Model and Format)
format, suitable for visualization in ParaView and other scientific
visualization tools. The application processes time-series simulation data and
generates XDMF metadata files that reference HDF5 data files.

## Current Implementation

**Note**: The current version is implemented in Julia (`xdmf.jl`) as a
prototype. This Julia implementation demonstrates the core functionality and
serves as a reference for the ongoing C++ port that leverages HighFive (HDF5
wrapper) and TinyXML2.
