find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB forms CONFIGURE_DEPENDS "view/forms/*")
file(GLOB sources CONFIGURE_DEPENDS
    "./*.cc" "./*.h"
    "view/*.*"
    "model/*.*"
    "model/*/*.*"
    "controller/*.*"
)

add_subdirectory(view/third_party/gifimage)

set(project_sources
    ${sources}
    ${forms}
)

qt_wrap_ui(UI_HEADERS ${forms}) # because of custom location
qt_add_executable(${PROJECT_NAME} ${project_sources})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        controller
        model
        view
        view/third_party
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::Core
    Qt6::OpenGLWidgets
    QtGifImage
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER smartcalc.bansheewaltz
    MACOSX_BUNDLE ON
    WIN32_EXECUTABLE ON
)

if(BUILD_TESTS)
    include(${CMAKE_SOURCE_DIR}/cmake/updateSubmodules.cmake)
    add_subdirectory(model/test/third_party/gtest)
    include(CTest)
    add_subdirectory(model/test)
endif()

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
