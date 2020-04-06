cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

set(PROJECT_NAME "TMS")
project(${PROJECT_NAME})

file(COPY ${ALL_REQUIRED_DLL} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH /usr/local/Cellar/qt/5.14.1)
# find Qt5 and required components
find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)
find_package(Qt5Widgets REQUIRED)

# find json c++ support and required components
find_package(nlohmann_json 3.2.0 REQUIRED)

# find SFML and required components
find_package(SFML COMPONENTS system window graphics REQUIRED)

# need this to include the auto generated ui_mainwindow.h
# file from the qt5_wrap_ui call below.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(project_sources
        resources/Cars/car_image_1.png
        resources/Cars/car_image_2.png
        resources/Cars/car_image_3.png
        src/sim/simulator/Engine.cpp
        src/sim/map/Intersection.cpp
        src/sim/map/Lane.cpp
        src/main.cpp
        src/sim/map/Road.cpp
        src/sim/simulator/Vehicle.cpp
        src/sim/map/Map.cpp
        src/ui/widgets/QsfmlCanvas.cpp
        src/sim/simulator/Settings.cpp
        src/ui/mainwindow.cpp
        src/sim/simulator/DataBox.cpp
        src/sim/simulator/DataBox.hpp
        src/sim/map/Route.cpp
        src/sim/map/Route.hpp
        src/sim/map/Phase.cpp
        src/sim/map/Phase.hpp
        src/sim/map/Light.cpp
        src/sim/map/Light.hpp
        src/sim/simulator/Simulation.cpp
        src/sim/simulator/Simulation.hpp
        src/ui/widgets/SimModel.cpp
        src/ui/widgets/SimModel.hpp
        src/sim/map/Cycle.cpp src/sim/map/Cycle.hpp)

set(project_headers
        src/sim/simulator/Engine.hpp
        src/sim/map/Intersection.hpp
        src/sim/map/Lane.hpp
        src/sim/map/Road.hpp
        src/sim/simulator/Vehicle.hpp
        src/sim/map/Map.hpp
        src/ui/widgets/QsfmlCanvas.hpp
        src/ui/mainwindow.h
        src/sim/simulator/Settings.hpp
        src/sim/simulator/DataBox.cpp
        src/sim/simulator/DataBox.hpp
        src/sim/map/Route.cpp
        src/sim/map/Route.hpp
        src/sim/map/Phase.cpp
        src/sim/map/Phase.hpp
        src/sim/map/Light.cpp
        src/sim/map/Light.hpp
        src/sim/simulator/Simulation.cpp
        src/sim/simulator/Simulation.hpp
        src/ui/widgets/SimModel.cpp
        src/ui/widgets/SimModel.hpp
        src/sim/map/Cycle.cpp src/sim/map/Cycle.hpp)

set(project_ui src/ui/mainwindow.ui)
# wrap the ui file to a c++ header
qt5_wrap_ui(ui_wrap ${project_ui})
qt5_wrap_cpp(moc_sources ${project_headers})

add_executable(TMS
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
    Qt5::Gui
    nlohmann_json::nlohmann_json
    )

