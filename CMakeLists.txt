add_library(ComponentLib INTERFACE)

add_subdirectory(Types)

target_link_libraries(ComponentLib 
    INTERFACE TypeLib
)
target_include_directories(ComponentLib 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/source
)

