cmake_minimum_required(VERSION 3.10)
project(cubo)

# Set C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O2")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -s")

# Directories for libraries
set(LIBRARIES_DIR lib)

# Set source files
set(SOURCES
    src/main.cpp
    src/glad.c 
    src/common.cpp
)

# Add the executable
add_executable(cubo ${SOURCES})

# Include directories for glad, GLFW, stb_image, glm, and the shader classes
include_directories(
    ${CMAKE_SOURCE_DIR}/lib/glad/include
    ${CMAKE_SOURCE_DIR}/include
)

# Set GLFW directory
set(GLFW_DIR "${LIBRARIES_DIR}/glfw")
# Set GLM directory
set(GLM_DIR "${LIBRARIES_DIR}/glm")

# Disabling unnecessary GLFW build options
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

# Add GLFW and GLM as a subdirectory (with the necessary options)
add_subdirectory("${GLFW_DIR}")
add_subdirectory("${GLM_DIR}") 

# Link libraries (GLFW and glad are directly included in the source)
target_link_libraries(cubo PRIVATE glfw)

# Include GLFW headers and other dependencies
target_include_directories(cubo PRIVATE "${GLFW_DIR}/include")
# target_include_directories(cubo PRIVATE "${GLM_DIR}/include")

# Add definitions for GLFW (disable OpenGL header inclusion by GLFW)
target_compile_definitions(cubo PRIVATE "GLFW_INCLUDE_NONE")
