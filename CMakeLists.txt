cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)

project(VGI)

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
${CMAKE_SOURCE_DIR}/src/*.c
${CMAKE_SOURCE_DIR}/src/*.cpp)

# Add header files
file(GLOB_RECURSE HEADER_FILES 
${CMAKE_SOURCE_DIR}/src/*.h
${CMAKE_SOURCE_DIR}/src/*.hpp)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
# Set the main porject as Visual Studio's Startup Project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Copy the assets folder to the build directory
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# Add Libraries
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glad)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glm)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ${CMAKE_SOURCE_DIR}/lib/glfw/include
)

include_directories(${CMAKE_SOURCE_DIR}/lib/stb_image)

target_link_directories(${PROJECT_NAME}
    PRIVATE lib/glfw/src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE glfw
    PRIVATE glad
    PRIVATE glm
)
