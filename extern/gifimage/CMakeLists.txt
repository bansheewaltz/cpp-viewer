option(ENABLE_CODE_ANALYSIS "Run code cppcheck" OFF)
cmake_minimum_required(VERSION 3.5)

# Path to cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

# disable extentions for pure C++
if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

enable_language(CXX)

if(ENABLE_CODE_ANALYSIS)
    find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)

    if(CMAKE_CXX_CPPCHECK)
        list(
            APPEND CMAKE_CXX_CPPCHECK
                "--enable=warning"
                "--inconclusive"
                "--force"
                "--inline-suppr"
                "--suppressions-list=${CMAKE_SOURCE_DIR}/CppCheckSuppressions.txt"
        )
    endif()
endif(ENABLE_CODE_ANALYSIS)

# dependency
add_subdirectory(giflib)

project(QtGifImage VERSION 1.0 LANGUAGES CXX)
find_package(Qt6Core)
find_package(Qt6Widgets)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(SRC qgifimage.cpp)

add_library(${PROJECT_NAME} ${SRC})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE giflib)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/giflib
    ${CMAKE_SOURCE_DIR}/include
)

set(CMAKE_AUTOMOC OFF)
