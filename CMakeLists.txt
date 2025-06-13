#Mother
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(USE_FOLDERS ON)
project(ProjetFinAnnee)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(nom_du_groupe Paul_Julius) #change the name 
add_subdirectory(Assets)
add_subdirectory(LibMath)
add_subdirectory(LibGL)
add_subdirectory(Game)


if (MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Game)
endif()