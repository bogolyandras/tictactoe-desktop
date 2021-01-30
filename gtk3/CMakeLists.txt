
cmake_minimum_required (VERSION 3.8)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source to this project's executable.
add_executable (TicTacToeGtk3 "src/gtkmain.cxx")
target_link_libraries(TicTacToeGtk3 PUBLIC ${GTK3_LIBRARIES})
target_link_libraries(TicTacToeGtk3 PUBLIC TicTacToeCore)

# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
target_include_directories(TicTacToeGtk3 PUBLIC ${GTK3_INCLUDE_DIRS})
target_include_directories(TicTacToeGtk3 PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/core/src")

add_definitions(${GTK_CFLAGS_OTHER})

# TODO: Add tests and install targets if needed.
