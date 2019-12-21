cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME "SimulatorSFML")
project(${PROJECT_NAME})
set(SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/libs/SFML")
file(GLOB ALL_REQUIRED_DLL "libs/required_dlls/*.dll")
file(COPY ${ALL_REQUIRED_DLL} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)
#link_libraries(stdc++fs) #link the filesystem

file(GLOB_RECURSE SCRF "src/*.cpp" "src/*.hpp")
add_executable(SimulatorSFML
        assets/Cars/car_image_1.png
        assets/Cars/car_image_2.png
        assets/Cars/car_image_3.png
        assets/Cars/car_image_4.png
        assets/Cars/car_image_5.png
        src/Engine.cpp
        src/Engine.hpp
        src/Intersection.cpp
        src/Intersection.hpp
        src/Lane.cpp
        src/Lane.hpp
        src/main.cpp
        src/Road.cpp
        src/Road.hpp
        src/Vehicle.cpp
        src/Vehicle.hpp)
find_package(SFML COMPONENTS system window graphics network audio REQUIRED)
target_link_libraries(${PROJECT_NAME} sfml-audio sfml-graphics sfml-window sfml-system)