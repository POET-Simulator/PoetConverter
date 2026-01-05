#include "SimFileList.hpp"

#include <filesystem>
#include <map>
#include <regex>
#include <stdexcept>

namespace fs = std::filesystem;

SimFileList::SimFileList(const std::string &directoryPath)
    : directoryPath(directoryPath) {
  // Validate that the directory exists
  if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath)) {
    throw std::runtime_error("Directory does not exist: " + directoryPath);
  }
  loadIterationFiles();
}

std::string SimFileList::getBaseName() const {
  fs::path dirPath(directoryPath);
  return dirPath.filename().string();
}

std::string SimFileList::getDirectoryPath() const { return directoryPath; }

const std::map<int, std::string> &SimFileList::getIterationFiles() const {
  return timestepToFileMap;
}

std::vector<int> SimFileList::getIterationNumbers() const {
  std::vector<int> numbers;
  numbers.reserve(timestepToFileMap.size());
  for (const auto &[iterNum, _] : timestepToFileMap) {
    numbers.push_back(iterNum);
  }
  // Already sorted because std::map keeps keys sorted
  return numbers;
}

void SimFileList::loadIterationFiles() {
  // Regular expression to match iter_*.qs2 files and extract the iteration
  // number Pattern: iter_<digits>.qs2
  std::regex iterPattern(R"(iter_(\d+)\.qs2)");

  for (const auto &entry : fs::directory_iterator(directoryPath)) {
    if (entry.is_regular_file()) {
      std::string filename = entry.path().filename().string();
      std::smatch match;

      if (std::regex_match(filename, match, iterPattern)) {
        // Extract the iteration number
        int iterNum = std::stoi(match[1].str());
        // Store the full path
        timestepToFileMap[iterNum] = entry.path().string();
      }
    }
  }
}
