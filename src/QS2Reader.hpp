/**
 * @file QS2Reader.hpp
 * @brief Reader for QS2 (qs2 package) serialized R data files
 */

#pragma once

#include <map>
#include <string>
#include <vector>

/**
 * @class QS2Reader
 * @brief Reads simulation data from QS2 format files
 *
 * This class provides functionality to read QS2 files (R serialization format)
 * containing simulation data organized as named columns. It uses the R qs2
 * package through RInside to deserialize the data.
 */
class QS2Reader {
public:
  /**
   * @brief Construct a new QS2Reader object
   * @param filePath Path to the QS2 file to read
   * @param rows Expected number of rows in the data grid
   * @param cols Expected number of columns in the data grid
   */
  QS2Reader(const std::string &filePath, int rows, int cols);

  /**
   * @brief Read data from the QS2 file
   * @return Map of column names to data vectors
   *
   * Deserializes the QS2 file and returns the data as a map where
   * keys are column/variable names and values are vectors of doubles.
   */
  std::map<std::string, std::vector<double>> read();

  /**
   * @brief Get the total number of data points
   * @return Total number of grid cells (rows * cols)
   */
  int getDataSize() const { return rows_ * cols_; }

  /**
   * @brief Get the number of rows in the grid
   * @return Number of rows
   */
  int getHeight() const { return rows_; }

  /**
   * @brief Get the number of columns in the grid
   * @return Number of columns
   */
  int getWidth() const { return cols_; }

private:
  std::string filePath_; ///< Path to the QS2 file
  const int rows_;       ///< Number of rows in the grid
  const int cols_;       ///< Number of columns in the grid

  /**
   * @brief Extract iteration number from filename
   * @param filename The filename to parse
   * @return Iteration number extracted from the filename
   */
  static int extractIterationNumber(const std::string &filename);

  /**
   * @brief Check if file is a valid QS2 file
   * @param filename The filename to check
   * @return true if file has .qs2 extension, false otherwise
   */
  static bool isQS2File(const std::string &filename);
};