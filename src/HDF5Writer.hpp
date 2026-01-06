#pragma once

#include <string>

class HDF5Writer {
public:
  // Writes all iteration data from the simulation directory to an HDF5 file.
  // - simDirectory: path containing iter_*.qs2 files
  // - outputPath: destination .h5 path
  // - rows/cols: grid dimensions (default 400x400)
  static void write(const std::string &simDirectory,
                    const std::string &outputPath, int rows = 400,
                    int cols = 400);
};
