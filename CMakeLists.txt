cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(OpenGL)

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)

# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

link_directories(${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glad)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glfw)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ${CMAKE_SOURCE_DIR}/lib/glfw/include
)

target_link_directories(${PROJECT_NAME}
    PRIVATE lib/glfw/src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE glfw
    PRIVATE glad
)