
cmake_minimum_required (VERSION 3.8)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source to this project's executable.
add_library (TicTacToeCore STATIC "layout.cxx" "table.cxx")

# TODO: Add tests and install targets if needed.
