#include "HDF5Writer.hpp"

#include <highfive/highfive.hpp>
#include <string>

HDF5Writer::HDF5Writer(const std::string &filePath, int rows, int cols)
    : File(filePath, File::Overwrite), rows_(rows), cols_(cols),
      dataSpace_({static_cast<size_t>(rows_), static_cast<size_t>(cols_)}) {
  // Configure dataset creation properties (compression + shuffle)
  props_.add(HighFive::Chunking(
      {static_cast<size_t>(rows_), static_cast<size_t>(cols_)}));
  props_.add(HighFive::Shuffle());
  props_.add(HighFive::Deflate(3));
}

void HDF5Writer::addDataset(
    int iteration, const std::map<std::string, std::vector<double>> &columns) {
  std::string groupName = "iteration_" + std::to_string(iteration);
  auto group = this->createGroup(groupName);

  for (const auto &[colName, colData] : columns) {
    auto dataset =
        group.createDataSet<double>(colName, this->dataSpace_, this->props_);
    dataset.write_raw(colData.data());
  }
}
