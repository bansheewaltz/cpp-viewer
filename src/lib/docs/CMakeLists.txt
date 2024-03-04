find_package(Doxygen)

if(DOXYGEN_FOUND)
  configure_file(Doxyfile.in Doxyfile)
  add_custom_target(docs ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green "You can find docs in an html format at ${CMAKE_CURRENT_BINARY_DIR}/html/index.html"
  )
endif()
