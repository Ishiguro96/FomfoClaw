cmake_minimum_required(VERSION 3.28)

project(
  FomfoClaw
  VERSION 0.0.1
  LANGUAGES CXX
)

# Set compiler specific settings
set (CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Let's nicely support folders in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)

add_executable(FomfoClaw main.cpp)

target_link_libraries(FomfoClaw 
                        PRIVATE
                          box2d::box2d
                          ImGui-SFML::ImGui-SFML
                          imgui::imgui
                          SFML::Audio
                          SFML::Graphics
                          SFML::Network
                          SFML::System
                          SFML::Window
                      )
