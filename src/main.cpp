/**
 * @file main.cpp
 * @brief Main entry point for POET to ParaView converter
 *
 * This application converts POET simulation output (QS2 format) to
 * HDF5 and XDMF formats for visualization in ParaView.
 */

#include "CLI/CLI.hpp"
#include "HDF5Writer.hpp"
#include "QS2Reader.hpp"
#include "SimFileList.hpp"
#include "XMLWriter.hpp"

#include <CLI/CLI.hpp>
#include <print>
#include <string>

/**
 * @brief Main function for POET to ParaView converter
 * @param argc Argument count
 * @param argv Argument vector
 * @return Exit code (0 for success, 1 for error)
 *
 * Usage: poet2paraview <input_dir> <rows> <cols>
 *
 * Arguments:
 * - input_dir: Directory containing simulation iteration files (iter_*.qs2)
 * - rows: Number of rows in the simulation grid
 * - cols: Number of columns in the simulation grid
 *
 * Output:
 * - <basename>.h5: HDF5 file containing all simulation data
 * - <basename>.xdmf: XDMF XML file for ParaView visualization
 */
int main(int argc, char *argv[]) {
  std::string simDirectory;
  int rows;
  int cols;

  CLI::App app{"POET to ParaView Converter"};
  app.add_option("input_dir", simDirectory, "Simulation directory")
      ->required()
      ->check(CLI::ExistingDirectory);

  app.add_option("rows", rows, "Number of rows in the simulation grid")
      ->required()
      ->check(CLI::PositiveNumber);

  app.add_option("cols", cols, "Number of columns in the simulation grid")
      ->required()
      ->check(CLI::PositiveNumber);

  CLI11_PARSE(app, argc, argv);

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

    HDF5Writer h5writer(h5path, rows, cols);
    XMLWriter xmlwriter(xdmf_path, h5path);
    for (const auto &iterFile : simList.getIterationFiles()) {
      int iterNum = iterFile.first;
      const std::string &filePath = iterFile.second;
      QS2Reader reader(filePath, rows, cols);
      auto columns = reader.read();
      h5writer.addDataset(iterNum, columns);

      // Write XDMF XML file
      xmlwriter.addDataset(iterNum, columns, rows, cols);
    }

    // HDF5Writer::write(simDirectory, h5path, 400, 400);
    // XMLWriter::write(xdmf_path, simDirectory, simList.getIterationNumbers(),
    //                  simList.getElementNames(), 400, 400);

    // std::println("HDF5 write completed successfully.");
    xmlwriter.save();
    std::println("XDMF write completed successfully.");
  } catch (const std::exception &e) {
    std::println(stderr, "Error: {}", e.what());
    return 1;
  }

  return 0;
}