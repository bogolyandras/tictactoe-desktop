set(CMAKE_CXX_STANDARD_REQUIRED true)

# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4 gtkmm-4.0)

# Add source to this project's executable.
link_directories(${GTK4_LIBRARY_DIRS})
add_definitions(${GTK4_CFLAGS_OTHER})
add_executable(TicTacToeGtk4 "src/gtkmain.cxx")
target_link_libraries(TicTacToeGtk4 PUBLIC ${GTK4_LIBRARIES})
target_link_libraries(TicTacToeGtk4 PUBLIC TicTacToeCore)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
target_include_directories(TicTacToeGtk4 PUBLIC ${GTK4_INCLUDE_DIRS})
target_include_directories(TicTacToeGtk4 PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/core/src")

# TODO: Add tests and install targets if needed.
