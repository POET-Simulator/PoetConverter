#include "HDF5Writer.hpp"
#include "SimFileList.hpp"

#include <filesystem>
#include <print>
#include <string>

int main(int argc, char *argv[]) {
  // Simulation directory (contains iter_*.qs2) and output .h5 path
  std::string simDirectory =
      argc > 1 ? argv[1] : "dolo_fgcs_3_skip"; // Default directory

  try {
    // Discover iterations first (also validates directory)
    SimFileList simList(simDirectory);

    // Compute default output path: <simDirectory>.h5 in current working dir
    std::string baseName = simList.getBaseName();
    std::string outputPath = baseName + ".h5";

    std::println("Simulation Directory: {}", simList.getDirectoryPath());
    std::println("Base Name: {}", baseName);
    std::println("Output HDF5: {}", outputPath);

    // Write HDF5 using HighFive
    HDF5Writer::write(simDirectory, outputPath, 400, 400);

    std::println("HDF5 write completed successfully.");
  } catch (const std::exception &e) {
    std::println(stderr, "Error: {}", e.what());
    return 1;
  }

  return 0;
}