
cmake_minimum_required (VERSION 3.8)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source to this project's executable.
add_library (TicTacToeCore STATIC "src/layout.cxx" "src/table.cxx" "src/ai/randomai.cxx" "src/ai/tableview.cxx")

# TODO: Add tests and install targets if needed.
