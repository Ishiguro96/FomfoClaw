set (LIB_NAME GameObjects)

set (SOURCES
      src/Claw.cpp
)

add_library (${LIB_NAME} STATIC ${SOURCES})

target_link_libraries (${LIB_NAME}
                        PRIVATE box2d::box2d
                        PRIVATE SFML::Audio
                        PRIVATE SFML::Graphics
                        PRIVATE SFML::Network
                        PRIVATE SFML::System
                        PRIVATE SFML::Window
                        PRIVATE Shapes
                        PRIVATE Constants
)

target_include_directories (${LIB_NAME}
                            PUBLIC include
                            PRIVATE include/${LIB_NAME}
                            PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
