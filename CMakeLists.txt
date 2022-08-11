# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project (tictactoe-desktop)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include sub-projects.
add_subdirectory(core)
if (WIN32)
    add_subdirectory(win32)
endif (WIN32)
if (UNIX)
    add_subdirectory(gtk4)
endif (UNIX)
