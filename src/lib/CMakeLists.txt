cmake_minimum_required(VERSION 3.16)

project(libowviewer LANGUAGES C)
get_directory_property(IS_SUBPROJECT PARENT_DIRECTORY)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_STATIC_LIBRARY_PREFIX "")

set(PUBLIC_INCLUDES
    include/owviewer.h
    include/owviewer/details.h
    include/owviewer/affine.h
)

add_library(${PROJECT_NAME} STATIC
    ${PUBLIC_INCLUDES}
    src/parsing.c
    src/processing.c
)

target_include_directories(${PROJECT_NAME} PUBLIC include include/owviewer)

# add_executable(test_time test/tests/time_test.cpp)
# target_link_libraries(test_time ${PROJECT_NAME})
# target_include_directories(test_time PRIVATE test/3rdparty)
option(ENABLE_DOCS "Build documenation" ON)

if(ENABLE_DOCS)
    add_subdirectory(docs)
endif()

option(ENABLE_TESTING "Enable testing" ON)
option(ENABLE_COVERAGE "Enable code coverage" ON)

if(ENABLE_TESTING)
    enable_testing()
    add_subdirectory(test/tests)
endif()

install(FILES ${PUBLIC_INCLUDES} DESTINATION include)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
