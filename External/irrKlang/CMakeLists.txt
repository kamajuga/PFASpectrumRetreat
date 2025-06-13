# irrKlang/CMakeLists.txt

add_library(irrKlang STATIC IMPORTED)

# Set the path to the imported .lib file (adjust name and arch if needed)
set_target_properties(irrKlang PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/irrKlang.lib
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/includes
)