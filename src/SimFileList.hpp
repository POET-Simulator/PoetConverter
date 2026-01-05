#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <vector>

class SimFileList {
public:
  SimFileList(const std::string &directoryPath);

  std::string getBaseName() const;

  std::string getDirectoryPath() const;

  // Get all iteration files (iter_*.qs2) as a map: iteration_number ->
  // full_path
  const std::map<int, std::string> &getIterationFiles() const;

  // Get sorted list of iteration numbers
  std::vector<int> getIterationNumbers() const;

private:
  std::string directoryPath;
  std::map<int, std::string> timestepToFileMap;

  void loadIterationFiles();
};