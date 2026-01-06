/**
 * @file RRuntime.hpp
 * @brief Singleton wrapper for R runtime environment
 */

#pragma once

#include <RInside.h>

/**
 * @class RRuntime
 * @brief Singleton wrapper for RInside providing R runtime functionality
 *
 * This class implements the Singleton pattern to ensure only one instance
 * of the R runtime exists throughout the application lifetime. It automatically
 * loads the qs2 library upon initialization.
 */
class RRuntime : public RInside {
public:
  /**
   * @brief Get the singleton instance of RRuntime
   * @return Reference to the singleton RRuntime instance
   *
   * This method ensures thread-safe lazy initialization of the
   * R runtime environment.
   */
  static RRuntime &getInstance() {
    static RRuntime instance;
    return instance;
  }

private:
  /**
   * @brief Private constructor initializing R runtime
   *
   * Constructs the RInside instance and loads the qs2 library.
   */
  RRuntime() : RInside() { this->parseEvalQ("library('qs2')"); };

  ~RRuntime() = default;
  RRuntime(const RRuntime &) = delete;
  RRuntime &operator=(const RRuntime &) = delete;
};