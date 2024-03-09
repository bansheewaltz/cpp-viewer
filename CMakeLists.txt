cmake_minimum_required(VERSION 3.21.1)

project(3dviewer VERSION 2.0 LANGUAGES C CXX)

option(BUILD_DOCS "Build documenation" OFF)
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_COVERAGE "Build code coverage" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(cmake/updateSubmodules.cmake)

add_subdirectory(src)

# Target uninstall
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake)
endif()

# Target dist
include(InstallRequiredSystemLibraries)
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-v${PROJECT_VERSION}-Source")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-v${PROJECT_VERSION}-${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)
