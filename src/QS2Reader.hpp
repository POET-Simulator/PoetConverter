#pragma once

#include <map>
#include <string>
#include <vector>

class QS2Reader {
public:
  // Constructor takes file path (can be a .qs2 file)
  QS2Reader(const std::string &filePath, int rows, int cols);

  std::map<std::string, std::vector<double>> read();

  // Get the number of data points
  int getDataSize() const { return rows_ * cols_; }

  int getHeight() const { return rows_; }
  int getWidth() const { return cols_; }

private:
  std::string filePath_;
  const int rows_;
  const int cols_;

  // Helper to extract iteration number from filename
  static int extractIterationNumber(const std::string &filename);

  // Helper to check if file is a valid .qs2 file
  static bool isQS2File(const std::string &filename);
};