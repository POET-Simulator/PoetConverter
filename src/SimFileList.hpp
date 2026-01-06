/**
 * @file SimFileList.hpp
 * @brief Directory scanner for simulation iteration files
 */

#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <vector>

/**
 * @class SimFileList
 * @brief Scans and manages simulation iteration files in a directory
 *
 * This class scans a specified directory for simulation iteration files
 * (files matching the pattern iter_*.qs2) and provides convenient access
 * to iteration numbers and file paths.
 */
class SimFileList {
public:
  /**
   * @brief Construct a new SimFileList object
   * @param directoryPath Path to the directory containing simulation files
   * @throws std::runtime_error if directory doesn't exist or contains no
   * iteration files
   */
  SimFileList(const std::string &directoryPath);

  /**
   * @brief Get the base name of the simulation directory
   * @return Base name (last component) of the directory path
   */
  std::string getBaseName() const;

  /**
   * @brief Get the full directory path
   * @return Full path to the simulation directory
   */
  std::string getDirectoryPath() const;

  /**
   * @brief Get all iteration files as a map
   * @return Map from iteration number to full file path
   *
   * Returns all iteration files (iter_*.qs2) found in the directory,
   * sorted by iteration number.
   */
  const std::map<int, std::string> &getIterationFiles() const;

  /**
   * @brief Get sorted list of iteration numbers
   * @return Vector of iteration numbers in ascending order
   */
  std::vector<int> getIterationNumbers() const;

private:
  std::string directoryPath; ///< Path to the simulation directory
  std::map<int, std::string>
      timestepToFileMap; ///< Map of iteration number to file path

  /**
   * @brief Load iteration files from the directory
   *
   * Scans the directory for files matching iter_*.qs2 pattern
   * and populates the internal map.
   */
  void loadIterationFiles();
};