/**
 * @file HDF5Writer.hpp
 * @brief HDF5 file writer for simulation data output
 */

#pragma once

#include "highfive/H5DataSpace.hpp"
#include <highfive/H5PropertyList.hpp>
#include <highfive/highfive.hpp>

#include <map>
#include <string>
#include <vector>

/**
 * @class HDF5Writer
 * @brief Writes simulation data to HDF5 format
 *
 * This class extends HighFive::File to provide specialized functionality
 * for writing simulation iteration data to HDF5 files. Each iteration
 * creates a group containing datasets for multiple columns of data.
 */
class HDF5Writer : public HighFive::File {
public:
  HDF5Writer() = delete;
  HDF5Writer(const HDF5Writer &) = delete;
  HDF5Writer &operator=(const HDF5Writer &) = delete;

  /**
   * @brief Construct a new HDF5Writer object
   * @param filePath Path to the output HDF5 file
   * @param rows Number of rows in the simulation grid
   * @param cols Number of columns in the simulation grid
   */
  HDF5Writer(const std::string &filePath, int rows, int cols);

  /**
   * @brief Add a dataset for a specific iteration
   * @param iteration The iteration number
   * @param columns Map of column names to data vectors
   *
   * Creates a group named "iter_<iteration>" and stores each column
   * as a separate dataset within that group.
   */
  void addDataset(int iteration,
                  const std::map<std::string, std::vector<double>> &columns);

private:
  const int rows_;                     ///< Number of rows in the grid
  const int cols_;                     ///< Number of columns in the grid
  HighFive::DataSetCreateProps props_; ///< HDF5 dataset creation properties
  HighFive::DataSpace dataSpace_;      ///< HDF5 dataspace for the grid
};
