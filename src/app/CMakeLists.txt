find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS OpenGLWidgets core gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS OpenGLWidgets core gui)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

qt_standard_project_setup()

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp mainwindow.hpp mainwindow.ui
    openglwidget.cpp openglwidget.hpp
    doubleslider.cpp doubleslider.hpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(untitled
        ${PROJECT_SOURCES}
    )
endif()

target_include_directories(${PROJECT_NAME} PRIVATE .)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::Core
    Qt6::OpenGLWidgets
    QtGifImage
    libowviewer
    d_classic
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER smartcalc.bansheewaltz
    MACOSX_BUNDLE ON
    WIN32_EXECUTABLE ON
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
