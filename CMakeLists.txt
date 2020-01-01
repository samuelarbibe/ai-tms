cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME "SimulatorSFML")
project(${PROJECT_NAME})
set(SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/libs/SFML")
file(GLOB ALL_REQUIRED_DLL "libs/required_dlls/*.dll")
file(COPY ${ALL_REQUIRED_DLL} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH /usr/local/Cellar/qt/5.14.0)
find_package(Qt5Widgets REQUIRED)
#/usr/local/Cellar/qt/5.14.0
#link_libraries(stdc++fs) #link the filesystem

file(GLOB_RECURSE SCRF "src/*.cpp" "src/*.hpp")
add_executable(SimulatorSFML
        resources/Cars/car_image_1.png
        resources/Cars/car_image_2.png
        resources/Cars/car_image_3.png
        resources/Cars/car_image_4.png
        resources/Cars/car_image_5.png
        src/Simulator/Engine.cpp
        src/Simulator/Engine.hpp
        src/Simulator/Map/Intersection.cpp
        src/Simulator/Map/Intersection.hpp
        src/Simulator/Map/Lane.cpp
        src/Simulator/Map/Lane.hpp
        src/main.cpp
        src/Simulator/Map/Road.cpp
        src/Simulator/Map/Road.hpp
        src/Simulator/Vehicle.cpp
        src/Simulator/Vehicle.hpp src/Simulator/Flags.hpp src/Simulator/Map/Map.cpp src/Simulator/Map/Map.hpp src/UI/QsfmlCanvas.cpp src/UI/QsfmlCanvas.hpp)
find_package(SFML COMPONENTS system window graphics network audio REQUIRED)
target_link_libraries(${PROJECT_NAME} sfml-audio sfml-graphics sfml-window sfml-system Qt5::Widgets)
#target_link_libraries(helloqt Qt5::Widgets)
