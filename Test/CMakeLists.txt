project(Test)

get_filename_component(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)
configure_file("root.hpp.in" root.hpp @ONLY)

add_executable(Test src/main.cpp)
target_link_libraries(Test PUBLIC OGL)

target_include_directories(Test PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")