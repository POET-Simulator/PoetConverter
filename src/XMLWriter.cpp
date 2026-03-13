#include "XMLWriter.hpp"

#include <stdexcept>
#include <string>
#include <tinyxml2.h>
#include <vector>

XMLWriter::XMLWriter(const std::string &outputPath,
                     const std::string &refh5path)
    : outputPath_(outputPath), refh5path_(refh5path) {
  doc_.InsertEndChild(doc_.NewDeclaration());

  tinyxml2::XMLElement *root = doc_.NewElement("Xdmf");
  root->SetAttribute("Version", "3.0");
  doc_.InsertEndChild(root);

  tinyxml2::XMLElement *domain = doc_.NewElement("Domain");
  root->InsertEndChild(domain);

  this->gridCollection_ = doc_.NewElement("Grid");
  this->gridCollection_->SetAttribute("Name", "Chemical_Evolution");
  this->gridCollection_->SetAttribute("GridType", "Collection");
  this->gridCollection_->SetAttribute("CollectionType", "Temporal");
  domain->InsertEndChild(this->gridCollection_);
}

void XMLWriter::save() {
  tinyxml2::XMLError eResult = doc_.SaveFile(outputPath_.c_str());
  if (eResult != tinyxml2::XML_SUCCESS) {
    throw std::runtime_error("Error saving XML file: " +
                             std::to_string(eResult));
  }
}

void XMLWriter::addDataset(
    int iterNum, const std::map<std::string, std::vector<double>> &columns,
    int rows, int cols) {
  std::string iterName = "iteration_" + std::to_string(iterNum);

  tinyxml2::XMLElement *grid = this->doc_.NewElement("Grid");
  grid->SetAttribute("Name", iterName.c_str());
  grid->SetAttribute("GridType", "Uniform");

  tinyxml2::XMLElement *time = this->doc_.NewElement("Time");
  time->SetAttribute("Value", std::to_string(iterNum).c_str());
  grid->InsertEndChild(time);

  // Add Topology and Geometry elements here as needed
  tinyxml2::XMLElement *topology = this->doc_.NewElement("Topology");
  topology->SetAttribute("TopologyType", "2DCoRectMesh");
  // XDMF expects dimensions in the order: rows cols
  topology->SetAttribute(
      "Dimensions",
      (std::to_string(rows) + " " + std::to_string(cols)).c_str());
  grid->InsertEndChild(topology);

  tinyxml2::XMLElement *geometry = this->doc_.NewElement("Geometry");
  geometry->SetAttribute("GeometryType", "ORIGIN_DXDY");

  tinyxml2::XMLElement *origin = this->doc_.NewElement("DataItem");
  origin->SetAttribute("Dimensions", "2");
  origin->SetAttribute("NumberType", "Float");
  origin->SetAttribute("Format", "XML");
  origin->SetText("0.0 0.0"); // Assuming origin at (0,0)
  geometry->InsertEndChild(origin);

  tinyxml2::XMLElement *spacing = this->doc_.NewElement("DataItem");
  spacing->SetAttribute("Dimensions", "2");
  spacing->SetAttribute("NumberType", "Float");
  spacing->SetAttribute("Format", "XML");
  spacing->SetText("-1.0 1.0"); // Assuming unit spacing
  geometry->InsertEndChild(spacing);

  grid->InsertEndChild(geometry);

  for (const auto &[elemName, _] : columns) {
    const std::string hdfPath =
        "/iteration_" + std::to_string(iterNum) + "/" + elemName;

    tinyxml2::XMLElement *attribute = doc_.NewElement("Attribute");
    attribute->SetAttribute("Name", elemName.c_str());
    attribute->SetAttribute("AttributeType", "Scalar");
    attribute->SetAttribute("Center", "Node");

    tinyxml2::XMLElement *dataItem = doc_.NewElement("DataItem");
    dataItem->SetAttribute("Format", "HDF");
    dataItem->SetAttribute("DataType", "Float");
    dataItem->SetAttribute("Precision", "8");
    dataItem->SetAttribute(
        "Dimensions",
        (std::to_string(rows) + " " + std::to_string(cols)).c_str());

    dataItem->SetText((this->refh5path_ + ":" + hdfPath).c_str());

    attribute->InsertEndChild(dataItem);
    grid->InsertEndChild(attribute);
  }

  this->gridCollection_->InsertEndChild(grid);
}
