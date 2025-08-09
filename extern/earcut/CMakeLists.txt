set (LIB_NAME EarCut)

add_library (${LIB_NAME} INTERFACE)

target_link_libraries (${LIB_NAME})

target_include_directories (${LIB_NAME}
                            INTERFACE include
)
