#include "HDF5Writer.hpp"
#include "QS2Reader.hpp"
#include "SimFileList.hpp"
#include "XMLWriter.hpp"

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
    std::string h5path = baseName + ".h5";
    std::string xdmf_path = baseName + ".xdmf";

    std::println("Simulation Directory: {}", simList.getDirectoryPath());
    std::println("Base Name: {}", baseName);
    std::println("Output HDF5: {}", h5path);

    // Write HDF5 using HighFive

    HDF5Writer h5writer(h5path, 400, 400);
    for (const auto &iterFile : simList.getIterationFiles()) {
      int iterNum = iterFile.first;
      const std::string &filePath = iterFile.second;
      QS2Reader reader(filePath, 400, 400);
      auto columns = reader.read();
      h5writer.addDataset(iterNum, columns);
    }

    // HDF5Writer::write(simDirectory, h5path, 400, 400);
    // XMLWriter::write(xdmf_path, simDirectory, simList.getIterationNumbers(),
    //                  simList.getElementNames(), 400, 400);

    std::println("HDF5 write completed successfully.");
  } catch (const std::exception &e) {
    std::println(stderr, "Error: {}", e.what());
    return 1;
  }

  return 0;
}