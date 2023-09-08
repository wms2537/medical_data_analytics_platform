cmake_minimum_required(VERSION 3.16)

set(CMAKE_OSX_ARCHITECTURES "arm64")

project(main VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts DataVisualization)
qt_standard_project_setup()

qt_add_executable(main
    mainwindow.ui
    mainwindow.cpp
    clickable_label.cpp
    multiselect_combobox.cpp
    heatmap_widget.cpp
    heatmap_legend.cpp
    legend.cpp
    main.cpp
)

target_link_libraries(main PRIVATE Qt6::Widgets Qt6::Charts Qt6::DataVisualization)

set_target_properties(main PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)