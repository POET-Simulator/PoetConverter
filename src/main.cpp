#include "QS2Reader.hpp"
#include "SimFileList.hpp"

#include <print>
#include <string>

int main(int argc, char *argv[]) {
  // Example usage
  std::string simDirectory =
      argc > 1 ? argv[1] : "dolo_fgcs_3_skip"; // Default directory

  try {
    // Create SimFileList object for the simulation directory
    SimFileList simList(simDirectory);

    // Display directory information
    std::println("Simulation Directory: {}", simList.getDirectoryPath());
    std::println("Base Name: {}", simList.getBaseName());

    // Get all iteration files
    const auto &iterFiles = simList.getIterationFiles();
    std::println("\nFound {} iteration files:\n", iterFiles.size());

    // Display iteration files
    for (const auto &[iterNum, filePath] : iterFiles) {
      std::println("  Iteration {}: {}", iterNum, filePath);
    }

    for (int iteration : {0, 1}) {
      QS2Reader reader(iterFiles.at(iteration), 400, 400);
      auto data = reader.read();
      std::println("\nData from iteration {}:", iteration);
      for (const auto &[key, values] : data) {
        std::println("  Key: {}, Data Size: {}", key, values.size());
      }
    }

    // // Example: Create Iteration objects
    // std::println("\nCreating Iteration objects:");
    // for (const auto &[iterNum, filePath] : iterFiles) {
    //   std::println("  Created Iteration #{}", iterNum);
    //   // Later we can load data: iter.getSimData()
    // }

  } catch (const std::exception &e) {
    std::println(stderr, "Error: {}", e.what());
    return 1;
  }

  return 0;
}