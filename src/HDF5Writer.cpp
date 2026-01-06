#include "HDF5Writer.hpp"
#include "QS2Reader.hpp"
#include "SimFileList.hpp"

#include <highfive/highfive.hpp>
#include <stdexcept>
#include <string>
#include <vector>

using HighFive::DataSetCreateProps;
using HighFive::DataSpace;
using HighFive::File;

namespace {
// Reorders a column-major flattened array (R/Julia style) into row-major.
std::vector<double> toRowMajor(const std::vector<double> &colMajor, int rows,
                               int cols) {
  if (static_cast<int>(colMajor.size()) != rows * cols) {
    throw std::runtime_error("Input size does not match rows*cols");
  }

  std::vector<double> rowMajor(colMajor.size());
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      // column-major index: c*rows + r
      // row-major index: r*cols + c
      rowMajor[static_cast<size_t>(r) * cols + c] =
          colMajor[static_cast<size_t>(c) * rows + r];
    }
  }
  return rowMajor;
}
} // namespace

void HDF5Writer::write(const std::string &simDirectory,
                       const std::string &outputPath, int rows, int cols) {
  // Discover iteration files
  SimFileList simList(simDirectory);
  const auto &iterFiles = simList.getIterationFiles();

  if (iterFiles.empty()) {
    throw std::runtime_error("No iter_*.qs2 files found in directory: " +
                             simDirectory);
  }

  // Create or overwrite the HDF5 file
  File h5(outputPath, File::Overwrite);

  // Configure dataset creation properties (compression + shuffle)
  DataSetCreateProps props;
  props.add(HighFive::Chunking(
      {static_cast<size_t>(rows), static_cast<size_t>(cols)}));
  props.add(HighFive::Shuffle());
  props.add(HighFive::Deflate(3));

  for (const auto &[iterNum, filePath] : iterFiles) {
    // Read data for this iteration
    QS2Reader reader(filePath, rows, cols);
    auto columns = reader.read();

    // Create group for iteration
    std::string groupName = "iteration_" + std::to_string(iterNum);
    auto group = h5.createGroup(groupName);

    for (const auto &[colName, colData] : columns) {
      auto dataset = group.createDataSet<double>(
          colName,
          DataSpace(std::vector<size_t>{static_cast<size_t>(rows),
                                        static_cast<size_t>(cols)}),
          props);
      dataset.write_raw(colData.data());
    }
  }
}
