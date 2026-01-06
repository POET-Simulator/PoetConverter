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

//   void HDF5Writer::write(const std::string &simDirectory,
//                          const std::string &outputPath, int rows, int cols) {
//     // Discover iteration files
//     SimFileList simList(simDirectory);
//     const auto &iterFiles = simList.getIterationFiles();

//     if (iterFiles.empty()) {
//       throw std::runtime_error("No iter_*.qs2 files found in directory: " +
//                                simDirectory);
//     }

//     // Create or overwrite the HDF5 file
//     File h5(outputPath, File::Overwrite);

//     // Configure dataset creation properties (compression + shuffle)
//     DataSetCreateProps props;
//     props.add(HighFive::Chunking(
//         {static_cast<size_t>(rows), static_cast<size_t>(cols)}));
//     props.add(HighFive::Shuffle());
//     props.add(HighFive::Deflate(3));

//     for (const auto &[iterNum, filePath] : iterFiles) {
//       // Read data for this iteration
//       QS2Reader reader(filePath, rows, cols);
//       auto columns = reader.read();

//       // Create group for iteration
//       std::string groupName = "iteration_" + std::to_string(iterNum);
//       auto group = h5.createGroup(groupName);

//       for (const auto &[colName, colData] : columns) {
//         auto dataset = group.createDataSet<double>(colName, this->dataSpace_,
//                                                    this->props_);
//         dataset.write_raw(colData.data());
//       }
//     }
//   }
