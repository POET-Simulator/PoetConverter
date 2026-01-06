#pragma once

#include <map>
#include <string>
#include <vector>

class QS2Reader {
public:
  // Constructor takes file path (can be a .qs2 file or directory with .qs2
  // files)
  QS2Reader(const std::string &filePath, int cols = 400, int rows = 400);

  std::map<std::string, std::vector<double>> read();

  // Set grid dimensions (default 400x400)
  void setDimensions(int cols, int rows);

  // Get the number of data points
  int getDataSize() const { return rows_ * cols_; }

  int getWidth() const { return rows_; }
  int getHeight() const { return cols_; }

private:
  std::string filePath_;
  int rows_;
  int cols_;

  // Helper to extract iteration number from filename
  static int extractIterationNumber(const std::string &filename);

  // Helper to check if file is a valid .qs2 file
  static bool isQS2File(const std::string &filename);
};