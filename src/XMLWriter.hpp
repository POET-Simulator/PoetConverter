#pragma once

#include <string>
#include <vector>

class XMLWriter {
public:
  static void write(const std::string &outputPath,
                    const std::string &simDirectory,
                    const std::vector<int> &iterations,
                    const std::vector<std::string> &elements, int rows,
                    int cols);
};
