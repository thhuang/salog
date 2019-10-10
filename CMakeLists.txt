cmake_minimum_required(VERSION 3.10)

set(this salog)

project(${this} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_BUILD_TYPE Release)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# Add source
include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(src)

# Add Google Test
include(ExternalProject)
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)
ExternalProject_Add(googletest
  GIT_REPOSITORY    https://github.com/google/googletest.git
  GIT_TAG           release-1.8.1
  CMAKE_ARGS        -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)
include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)

# Add test
enable_testing()
add_subdirectory(test)