set(CMAKE_CXX_STANDARD_REQUIRED true)

# Add source to this project's executable.
add_library (TicTacToeCore STATIC "src/layout.cxx" "src/table.cxx" "src/ai/heuristicai.cxx")

# TODO: Add tests and install targets if needed.
