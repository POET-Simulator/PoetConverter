#include "QS2Reader.hpp"
#include "RRuntime.hpp"
#include <Rcpp/DataFrame.h>
#include <regex>
#include <stdexcept>
#include <string>

QS2Reader::QS2Reader(const std::string &filePath, int rows, int cols)
    : filePath_(filePath), rows_(rows), cols_(cols) {}

int QS2Reader::extractIterationNumber(const std::string &filename) {
  // Extract number from "iter_XXXXX.qs2"
  std::regex iter_regex(R"(iter_(\d+)\.qs2)");
  std::smatch match;

  if (std::regex_search(filename, match, iter_regex) && match.size() > 1) {
    return std::stoi(match[1].str());
  }

  return -1; // Invalid filename
}

bool QS2Reader::isQS2File(const std::string &filename) {
  return filename.size() > 4 && filename.substr(filename.size() - 4) == ".qs2";
}

std::map<std::string, std::vector<double>> QS2Reader::read() {
  std::map<std::string, std::vector<double>> data_map;

  try {
    RInside &R = RRuntime::getInstance();

    // Read the QS2 file using R
    std::string r_cmd = "data <- qs2::qs_read(\"" + filePath_ + "\")";
    R.parseEvalQ(r_cmd);

    // Check if data is a data frame (initial state) or list with component C
    R.parseEvalQ("if (is.data.frame(data)) { df <- data } "
                 "else { df <- data$C }");

    // Convert R data frame to C++ map
    Rcpp::DataFrame result = R.parseEval("df");
    std::vector<std::string> col_names =
        Rcpp::as<std::vector<std::string>>(result.names());

    for (const auto &col_name : col_names) {
      Rcpp::NumericVector col_data =
          Rcpp::as<Rcpp::NumericVector>(result[col_name]);
      data_map[col_name] = Rcpp::as<std::vector<double>>(col_data);

      if (data_map[col_name].size() != rows_ * cols_) {
        throw std::runtime_error("Column " + col_name + " size (" +
                                 std::to_string(data_map[col_name].size()) +
                                 ") does not match expected dimensions (" +
                                 std::to_string(rows_ * cols_) + ")");
      }
    }

    return data_map;
  } catch (const std::exception &e) {
    throw std::runtime_error("Error reading QS2 file: " +
                             std::string(e.what()));
  }
}