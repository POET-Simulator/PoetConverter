cmake_minimum_required(VERSION 3.20)

project(PoetToParaView LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    tinyxml2
    GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
    GIT_TAG        11.0.0
)

FetchContent_Declare(
  highfive
  QUIET
  GIT_REPOSITORY https://github.com/highfive-devs/highfive.git
  GIT_TAG v3.2.0
)

FetchContent_Declare(
    cli11
    QUIET
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.6.1
)

FetchContent_MakeAvailable(tinyxml2)
FetchContent_MakeAvailable(highfive)
FetchContent_MakeAvailable(cli11)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
find_package(RRuntime REQUIRED)

add_executable(poet2paraview 
  src/main.cpp 
  src/SimFileList.cpp
  src/QS2Reader.cpp
  src/HDF5Writer.cpp
  src/XMLWriter.cpp
)

target_link_libraries(poet2paraview PRIVATE tinyxml2::tinyxml2 RRuntime HighFive::HighFive CLI11::CLI11)