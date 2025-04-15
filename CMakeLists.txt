cmake_minimum_required(VERSION 3.10)
project(ft_vox VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# Path to dependencies
set(GLFW_DIR "${CMAKE_SOURCE_DIR}/lib/glfw")
set(GLM_DIR "${CMAKE_SOURCE_DIR}/lib/glm")
set(STB_DIR "${CMAKE_SOURCE_DIR}/lib/stb")
set(GLEW_DIR "${CMAKE_SOURCE_DIR}/lib/glew")

# Include directories
include_directories(
    ${GLFW_DIR}/include
    ${GLM_DIR}
    ${STB_DIR}
    ${GLEW_DIR}/include
    ${CMAKE_SOURCE_DIR}/include
)

# Define source files
file(GLOB SOURCES "src/*.cpp")

# Create the executable
add_executable(ft_vox ${SOURCES})

# Use GLEW_STATIC globally
add_definitions(-DGLEW_STATIC)

# Platform-specific linking
if(WIN32)
    message(STATUS "Configuring for Windows")

    # Add library search paths
    target_link_directories(ft_vox PRIVATE
        ${GLFW_DIR}/build/src/Release
        ${GLEW_DIR}/lib/Release/x64
    )

    # Link Windows libs
    target_link_libraries(ft_vox
        glfw3.lib
        glew32s.lib
        opengl32.lib
    )

elseif(UNIX)
    message(STATUS "Configuring for Unix/Linux")

    # Find OpenGL and link
    find_package(OpenGL REQUIRED)

    # Link Unix libs
    target_link_libraries(ft_vox
        ${OPENGL_gl_LIBRARY}
        pthread
        dl
        m
        GLEW  # Assumes GLEW is installed via system package manager or submodule
        glfw
    )
endif()
