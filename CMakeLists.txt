cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

set(PROJECT_NAME "SimulatorSFML")
project(${PROJECT_NAME})

file(COPY ${ALL_REQUIRED_DLL} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH /usr/local/Cellar/qt/5.14.0)
# find Qt5 and required components
find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)
find_package(Qt5Widgets REQUIRED)

# find SFML and required components
find_package(SFML COMPONENTS system window graphics REQUIRED)

# need this to include the auto generated ui_mainwindow.h
# file from the qt5_wrap_ui call below.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(project_sources
    resources/Cars/car_image_1.png
    resources/Cars/car_image_2.png
    resources/Cars/car_image_3.png
    resources/Cars/car_image_4.png
    resources/Cars/car_image_5.png
    src/Simulator/Engine.cpp
    src/Simulator/Map/Intersection.cpp
    src/Simulator/Map/Lane.cpp
    src/main.cpp
    src/Simulator/Map/Road.cpp
    src/Simulator/Vehicle.cpp
    src/Simulator/Map/Map.cpp
    src/UI/Widgets/QsfmlCanvas.cpp
    src/UI/mainwindow.cpp)

set(project_headers
    src/Simulator/Engine.hpp
    src/Simulator/Map/Intersection.hpp
    src/Simulator/Map/Lane.hpp
    src/Simulator/Map/Road.hpp
    src/Simulator/Vehicle.hpp
        src/Simulator/Map/Map.hpp
    src/UI/Widgets/QsfmlCanvas.hpp
    src/UI/mainwindow.h src/Simulator/Settings.cpp src/Simulator/Settings.hpp)

set(project_ui src/UI/mainwindow.ui)
# wrap the ui file to a c++ header
qt5_wrap_ui(ui_wrap ${project_ui})
qt5_wrap_cpp(moc_sources ${project_headers})

add_executable(SimulatorSFML
        ${project_sources}
        ${project_headers}
        ${ui_wrap}
        ${moc_sources}
        )
message(${ui_wrap})

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    sfml-graphics
    sfml-window
    sfml-system
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui)

