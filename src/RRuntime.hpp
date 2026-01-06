#pragma once

#include <RInside.h>

class RRuntime : public RInside {
public:
  static RRuntime &getInstance() {
    static RRuntime instance;
    return instance;
  }

private:
  RRuntime() : RInside() { this->parseEvalQ("library('qs2')"); };
  ~RRuntime() = default;
  RRuntime(const RRuntime &) = delete;
  RRuntime &operator=(const RRuntime &) = delete;
};