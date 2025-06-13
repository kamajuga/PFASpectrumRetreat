#LibGL

add_subdirectory(Dependencies)
add_subdirectory(Resources)
add_subdirectory(LowRenderer)
add_subdirectory(Physics)
add_subdirectory(DataStructure)


# Create an interface library to aggregate all sub-libraries
add_library(LibGL INTERFACE)

# Optional: For Visual Studio folders
set_target_properties(LibGL PROPERTIES FOLDER "LibGL")

# Include directories for consumers of ModernOpenGL (interface)
target_include_directories(LibGL INTERFACE
	${DEPENDENCIES_INCLUDE_DIR}
	${RESOURCES_INCLUDE_DIR}
	${DATA_STRUCT_INCLUDE_DIR}
	${LOW_RENDERER_INCLUDE_DIR}
	${PHYSICS_INCLUDE_DIR}
)

# Link all sub-libraries to the interface ModernOpenGL target
target_link_libraries(LibGL INTERFACE
	${DEPENDENCIES_NAME}
	${RESOURCES_NAME} 
	${DATA_STRUCT}
	${LOW_RENDERER}
	${PHYSICS_NAME}
)


# Export ModernOpenGL target to parent scope
set(LIBGL LibGL PARENT_SCOPE)
