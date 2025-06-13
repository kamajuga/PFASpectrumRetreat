#Game
# set the target name (the visual studio project)
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(EXE_NAME ${CURRENT_FOLDER_NAME})
add_executable(${EXE_NAME})



###############################
#                             #
# Sources                     #
#                             #
###############################

file(GLOB_RECURSE PROJECT_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.inl

	${CMAKE_CURRENT_SOURCE_DIR}/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/*.cc
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/*.c++

	${CMAKE_CURRENT_SOURCE_DIR}/*.txt
	${CMAKE_CURRENT_SOURCE_DIR}/*.md)

target_sources(${EXE_NAME} PRIVATE ${PROJECT_FILES})
target_include_directories(${EXE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)


###############################
#                             #
# Projet Modern openGL        #
#                             #
###############################

# Déclaration des include dirs
set(DATA_STRUCT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/LibGL/DataStructure/Header)
set(LOW_RENDERER_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/LibGL/LowRenderer/Header)
set(PHYSICS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/LibGL/Physics/Header)
set(RESOURCES_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/LibGL/Resources/Header)
set(DEPENDENCIES_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/LibGL/Dependencies/Header)

target_include_directories(${EXE_NAME} PRIVATE ${DATA_STRUCT_INCLUDE_DIR})
target_include_directories(${EXE_NAME} PRIVATE ${LOW_RENDERER_INCLUDE_DIR})
target_include_directories(${EXE_NAME} PRIVATE ${PHYSICS_INCLUDE_DIR})
target_include_directories(${EXE_NAME} PRIVATE ${RESOURCES_INCLUDE_DIR})
target_include_directories(${EXE_NAME} PRIVATE ${DEPENDENCIES_INCLUDE_DIR})


# Déclaration des targets
set(DATA_STRUCT DataStructure)
set(LOW_RENDERER LowRenderer)
set(PHYSICS_NAME Physics)
set(RESOURCES_NAME Resources)
set(DEPENDENCIES_NAME Dependencies)

target_link_libraries(${EXE_NAME} PRIVATE ${DATA_STRUCT})
target_link_libraries(${EXE_NAME} PRIVATE ${LOW_RENDERER})
target_link_libraries(${EXE_NAME} PRIVATE ${PHYSICS_NAME})
target_link_libraries(${EXE_NAME} PRIVATE ${RESOURCES_NAME})
target_link_libraries(${EXE_NAME} PRIVATE ${DEPENDENCIES_NAME})
target_link_libraries(${EXE_NAME} PRIVATE glfw3)
target_link_libraries(${EXE_NAME} PRIVATE opengl32)
target_link_libraries(${EXE_NAME} PRIVATE user32)
target_link_libraries(${EXE_NAME} PRIVATE gdi32)
target_link_libraries(${EXE_NAME} PRIVATE shell32)




###############################
#                             #
# Projet LibMath              #
#                             #
###############################

# FetchContent_Declare(
	# LibMath
	# GIT_REPOSITORY	ssh://git@git.isartintra.com:2424/2022/Lib_De_Math/${nom_du_groupe}.git  #change the nom_du_groupe 
	# GIT_SHALLOW		ON
# )

set(BUILD_UNIT_TEST OFF)

# FetchContent_MakeAvailable(LibMath)
target_include_directories(${EXE_NAME} PRIVATE ${LIB_INCLUDE_DIR})
target_link_libraries(${EXE_NAME} PRIVATE ${LIB_NAME})

#################################
#        irrKlang               #
#################################

# Add irrKlang include directory
target_include_directories(${EXE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/External/irrKlang/includes)

# Link irrKlang library
target_link_libraries(${EXE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/External/irrKlang/libs/irrKlang.lib)

# Copy irrKlang.dll to the output directory (Debug or Release)
add_custom_command(TARGET ${EXE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/External/irrKlang/libs/irrKlang.dll"
        "$<TARGET_FILE_DIR:${EXE_NAME}>")

#################################
#        Dear ImGui Setup       #
#################################

# Définir le dossier où est ImGui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/External/ImGui)

# Fichiers source d'ImGui
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Include directories
target_include_directories(${EXE_NAME} PRIVATE ${IMGUI_DIR})
target_include_directories(${EXE_NAME} PRIVATE ${IMGUI_DIR}/backends)

# Ajouter les fichiers sources au projet
target_sources(${EXE_NAME} PRIVATE ${IMGUI_SOURCES})

add_custom_command(TARGET ${EXE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Assets/Shaders"
        "$<TARGET_FILE_DIR:${EXE_NAME}>/Shaders"
)

add_custom_command(TARGET ${EXE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Assets/Meshes"
        "$<TARGET_FILE_DIR:${EXE_NAME}>/Meshes"
)

add_custom_command(TARGET ${EXE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Assets/Sounds"
        "$<TARGET_FILE_DIR:${EXE_NAME}>/Sounds"
)
