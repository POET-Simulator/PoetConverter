#include "XMLWriter.hpp"

#include <stdexcept>
#include <string>
#include <tinyxml2.h>
#include <vector>

void XMLWriter::write(const std::string &outputPath,
                      const std::string &simDirectory,
                      const std::vector<int> &iterations,
                      const std::vector<std::string> &elements, int rows,
                      int cols) {
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLElement *root = doc.NewElement("Xdmf");
  root->SetAttribute("Version", "3.0");
  doc.InsertFirstChild(root);

  tinyxml2::XMLElement *domain = doc.NewElement("Domain");
  root->InsertEndChild(domain);

  tinyxml2::XMLElement *gridCollection = doc.NewElement("Grid");
  gridCollection->SetAttribute("Name", "Chemical_Evolution");
  gridCollection->SetAttribute("GridType", "Collection");
  gridCollection->SetAttribute("CollectionType", "Temporal");
  domain->InsertEndChild(gridCollection);

  for (int iterNum : iterations) {
    std::string iterName = "iteration_" + std::to_string(iterNum);

    tinyxml2::XMLElement *grid = doc.NewElement("Grid");
    grid->SetAttribute("Name", iterName.c_str());
    grid->SetAttribute("GridType", "Uniform");

    tinyxml2::XMLElement *time = doc.NewElement("Time");
    time->SetAttribute("Value", std::to_string(iterNum).c_str());
    grid->InsertEndChild(time);

    // Add Topology and Geometry elements here as needed
    tinyxml2::XMLElement *topology = doc.NewElement("Topology");
    topology->SetAttribute("TopologyType", "2DCoRectMesh");
    // XDMF expects dimensions in the order: rows cols
    topology->SetAttribute(
        "Dimensions",
        (std::to_string(rows) + " " + std::to_string(cols)).c_str());
    grid->InsertEndChild(topology);

    tinyxml2::XMLElement *geometry = doc.NewElement("Geometry");
    geometry->SetAttribute("GeometryType", "ORIGIN_DXDY");

    tinyxml2::XMLElement *origin = doc.NewElement("DataItem");
    origin->SetAttribute("Dimensions", "2");
    origin->SetAttribute("NumberType", "Float");
    origin->SetAttribute("Format", "XML");
    origin->SetText("0.0 0.0"); // Assuming origin at (0,0)
    geometry->InsertEndChild(origin);

    tinyxml2::XMLElement *spacing = doc.NewElement("DataItem");
    spacing->SetAttribute("Dimensions", "2");
    spacing->SetAttribute("NumberType", "Float");
    spacing->SetAttribute("Format", "XML");
    spacing->SetText("1.0 1.0"); // Assuming unit spacing
    geometry->InsertEndChild(spacing);

    grid->InsertEndChild(geometry);

    for (const auto &elemName : elements) {
      const std::string hdfPath = simDirectory + "/iteration_" +
                                  std::to_string(iterNum) + "/" + elemName;

      tinyxml2::XMLElement *attribute = doc.NewElement("Attribute");
      attribute->SetAttribute("Name", elemName.c_str());
      attribute->SetAttribute("AttributeType", "Scalar");
      attribute->SetAttribute("Center", "Node");

      tinyxml2::XMLElement *dataItem = doc.NewElement("DataItem");
      dataItem->SetAttribute("Format", "HDF");
      dataItem->SetAttribute("DataType", "Float");
      dataItem->SetAttribute("Precision", "8");
      dataItem->SetAttribute(
          "Dimensions",
          (std::to_string(rows) + " " + std::to_string(cols)).c_str());
      dataItem->SetText((outputPath + ":" + hdfPath).c_str());

      attribute->InsertEndChild(dataItem);
      grid->InsertEndChild(attribute);
    }

    gridCollection->InsertEndChild(grid);
  }

  // Write the XML document to file
  tinyxml2::XMLError eResult = doc.SaveFile(outputPath.c_str());
  if (eResult != tinyxml2::XML_SUCCESS) {
    throw std::runtime_error("Error saving XML file: " +
                             std::to_string(eResult));
  }
}
