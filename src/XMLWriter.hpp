#pragma once

#include "tinyxml2.h"
#include <map>
#include <string>
#include <vector>

class XMLWriter {
public:
  XMLWriter() = delete;
  XMLWriter(const XMLWriter &) = delete;
  XMLWriter &operator=(const XMLWriter &) = delete;

  XMLWriter(const std::string &outputPath, const std::string &refh5path);
  ~XMLWriter() = default;

  void addDataset(int iterNum,
                  const std::map<std::string, std::vector<double>> &columns,
                  int rows, int cols);

  void save();

private:
  tinyxml2::XMLDocument doc_;
  tinyxml2::XMLElement *gridCollection_;

  const std::string outputPath_;
  const std::string refh5path_;
};
