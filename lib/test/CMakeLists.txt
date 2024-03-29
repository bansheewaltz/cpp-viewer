set(TEST_EXE utests)

file(GLOB TEST_SUITES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cc)
message(${TEST_SUITES})
add_executable(${TEST_EXE} ${TEST_SUITES} ../lowlevel_parser.c)

# add_subdirectory(third_party/fast_obj)
target_link_libraries(${TEST_EXE} GTest::gtest_main)
target_include_directories(${TEST_EXE} PRIVATE ../ third_party/cglm/include/)
target_compile_options(${TEST_EXE} PRIVATE -Wall -Wextra -Wno-deprecated-declarations -Wno-sign-compare)

include(GoogleTest)
gtest_discover_tests(${TEST_EXE})

add_custom_target(check
    DEPENDS ${TEST_EXE}
    COMMAND ${CMAKE_CTEST_COMMAND}
    --output-on-failure
    --rerun-failed
    || exit 0
    COMMAND ${CMAKE_COMMAND}
    -E cmake_echo_color --cyan
  "Test logs can be found in ${CMAKE_CURRENT_BINARY_DIR}/Testing/Temporary/LastTest.log"
)

find_program(VALGRIND_COMMAND valgrind)

if(VALGRIND_COMMAND)
  add_custom_target(memcheck
    DEPENDS ${TEST_EXE}
    COMMAND ${CMAKE_CTEST_COMMAND}
    --force-new-ctest-process
    --test-action memcheck
    COMMAND ${CMAKE_COMMAND}
    -E cat "${CMAKE_BINARY_DIR}/Testing/Temporary/MemoryChecker.*.log"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
  )
endif()

find_program(GCOVR_COMMAND gcovr)

# message(fuck ${CMAKE_CXX_COMPILER_ID})
if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  set(GCOV_EXE "/Library/Developer/CommandLineTools/usr/bin/llvm-cov gcov")
else()
  set(GCOV_EXE "gcov")
endif()

if(GCOVR_COMMAND)
  set(COVERAGE_DIR ${PROJECT_BINARY_DIR}/coverage)
  target_compile_options(${TEST_EXE} PRIVATE --coverage)
  target_link_options(${TEST_EXE} PRIVATE --coverage)

  # target_link_libraries(${PROJECT_NAME} INTERFACE gcov)
  add_custom_target(coverage
    COMMAND ${GCOVR_COMMAND}
    --gcov-executable ${GCOV_EXE}
    --txt
    --html-details=${COVERAGE_DIR}/report.html
    --html-self-contained
    --decisions
    --root=${PROJECT_SOURCE_DIR}/
    --filter=${PROJECT_SOURCE_DIR}/lib
    --exclude=${PROJECT_SOURCE_DIR}/lib/test
    --object-directory=${PROJECT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND}
    -E cmake_echo_color --cyan "Html coverage report can be found in ${COVERAGE_DIR}/report.html"
    DEPENDS create_coverage_dir
  )
  add_custom_target(create_coverage_dir
    COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_DIR})
else()
  set(ENABLE_COVERAGE OFF)
  message(WARNING "gcovr program required for code coverage analysis was not found.")
endif()
