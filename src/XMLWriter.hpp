/**
 * @file XMLWriter.hpp
 * @brief XDMF XML file writer for ParaView visualization
 */

#pragma once

#include "tinyxml2.h"
#include <map>
#include <string>
#include <vector>

/**
 * @class XMLWriter
 * @brief Writes XDMF format XML files for visualization in ParaView
 *
 * This class creates XDMF (eXtensible Data Model and Format) XML files
 * that reference HDF5 data for visualization in ParaView. XDMF provides
 * a lightweight description of the data structure and geometry.
 */
class XMLWriter {
public:
  XMLWriter() = delete;
  XMLWriter(const XMLWriter &) = delete;
  XMLWriter &operator=(const XMLWriter &) = delete;

  /**
   * @brief Construct a new XMLWriter object
   * @param outputPath Path for the output XDMF XML file
   * @param refh5path Path to the referenced HDF5 file (relative or absolute)
   *
   * Initializes the XDMF document structure with the appropriate headers
   * and creates a temporal grid collection.
   */
  XMLWriter(const std::string &outputPath, const std::string &refh5path);

  ~XMLWriter() = default;

  /**
   * @brief Add a dataset for a specific iteration
   * @param iterNum The iteration number
   * @param columns Map of attribute names to data vectors
   * @param rows Number of rows in the grid
   * @param cols Number of columns in the grid
   *
   * Adds a grid element to the temporal collection with geometry
   * and attribute information referencing the HDF5 file.
   */
  void addDataset(int iterNum,
                  const std::map<std::string, std::vector<double>> &columns,
                  int rows, int cols);

  /**
   * @brief Save the XDMF document to file
   *
   * Writes the complete XDMF XML document to the output path
   * specified in the constructor.
   */
  void save();

private:
  tinyxml2::XMLDocument doc_; ///< TinyXML2 document object
  tinyxml2::XMLElement
      *gridCollection_; ///< Pointer to the temporal grid collection element

  const std::string outputPath_; ///< Path for output XDMF file
  const std::string refh5path_;  ///< Path to referenced HDF5 file
};
