#pragma once

#include "highfive/H5DataSpace.hpp"
#include <highfive/H5PropertyList.hpp>
#include <highfive/highfive.hpp>

#include <map>
#include <string>
#include <vector>

class HDF5Writer : public HighFive::File {
public:
  HDF5Writer() = delete;
  HDF5Writer(const HDF5Writer &) = delete;
  HDF5Writer &operator=(const HDF5Writer &) = delete;

  HDF5Writer(const std::string &filePath, int rows, int cols);

  void addDataset(int iteration,
                  const std::map<std::string, std::vector<double>> &columns);

private:
  const int rows_;
  const int cols_;
  HighFive::DataSetCreateProps props_;
  HighFive::DataSpace dataSpace_;
};
