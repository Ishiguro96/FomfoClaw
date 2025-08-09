cmake_minimum_required (VERSION 3.28)

project(FomfoClaw
          VERSION 0.0.1
          LANGUAGES CXX
)

# Set compiler specific settings
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS ON)

find_package (SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)
find_package (box2d CONFIG REQUIRED)
find_package (imgui CONFIG REQUIRED)
find_package (ImGui-SFML CONFIG REQUIRED)

add_executable (FomfoClaw main.cpp)

target_link_libraries (FomfoClaw
                        PRIVATE box2d::box2d
                        PRIVATE ImGui-SFML::ImGui-SFML
                        PRIVATE imgui::imgui
                        PRIVATE SFML::Audio
                        PRIVATE SFML::Graphics
                        PRIVATE SFML::Network
                        PRIVATE SFML::System
                        PRIVATE SFML::Window
                        PRIVATE Shapes
)

add_subdirectory (extern)
add_subdirectory (libs)
