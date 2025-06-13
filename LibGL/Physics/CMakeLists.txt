#Physics

set(PHYSICS_NAME Physics) #Local variable only

file(GLOB_RECURSE PROJECT_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Header/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp
)

add_library(${PHYSICS_NAME})

target_sources(${PHYSICS_NAME} PRIVATE ${PROJECT_FILES})

target_include_directories(${PHYSICS_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Header)
target_include_directories(${PHYSICS_NAME} PRIVATE ${LIB_INCLUDE_DIR})
target_include_directories(${PHYSICS_NAME} PRIVATE ${RESOURCES_INCLUDE_DIR})
target_include_directories(${PHYSICS_NAME} PRIVATE ${LOW_RENDERER_INCLUDE_DIR})
target_include_directories(${PHYSICS_NAME} PRIVATE ${DEPENDENCIES_INCLUDE_DIR})


target_link_libraries(${PHYSICS_NAME} PRIVATE ${LIB_NAME})
target_link_libraries(${PHYSICS_NAME} PRIVATE ${RESOURCES_NAME})
target_link_libraries(${PHYSICS_NAME} PRIVATE ${LOW_RENDERER})
target_link_libraries(${PHYSICS_NAME} PRIVATE ${DEPENDENCIES_NAME})

set(PHYSICS_NAME ${PHYSICS_NAME} PARENT_SCOPE) #outer scope variable only

set(PHYSICS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Header PARENT_SCOPE)
#This command sets the LIB_INCLUDE_DIR variable to the path of the 'include' directory