cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

project(OGL VERSION 0.6.0 LANGUAGES CXX)

configure_file("include/version.hpp.in" version.hpp @ONLY)
add_library(OGL STATIC "src/OGL.cpp" "src/Program/Program.cpp" "src/VAO/VAO.cpp")

set(GLFW_LIBRARY_TYPE STATIC)
set(GLFW_BUILD_SHARED_LIBS OFF)

add_subdirectory("${CMAKE_SOURCE_DIR}/glew-2.1.0/build/cmake")
add_subdirectory("${CMAKE_SOURCE_DIR}/glfw-3.4")
add_subdirectory("${CMAKE_SOURCE_DIR}/glm")

target_link_libraries(OGL PUBLIC glew_s glfw glm)

target_include_directories(OGL PUBLIC 
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/glew-2.1.0/include"
    "${CMAKE_SOURCE_DIR}/glfw-3.4/include"
    "${CMAKE_CURRENT_BINARY_DIR}"
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(OGL PUBLIC _OGL_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(OGL PUBLIC _OGL_RELEASE)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    target_compile_definitions(OGL PUBLIC _OGL_32BIT)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_compile_definitions(OGL PUBLIC _OGL_64BIT)
endif()

add_subdirectory("${CMAKE_SOURCE_DIR}/Test")