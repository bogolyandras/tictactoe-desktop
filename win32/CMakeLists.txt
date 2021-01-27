
cmake_minimum_required (VERSION 3.8)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source to this project's executable.
add_executable (TicTacToeWin32 WIN32 "src/winmain.cxx" "src/direct2d.cxx")
target_link_libraries(TicTacToeWin32 PUBLIC ComCtl32.lib)
target_link_libraries(TicTacToeWin32 PUBLIC TicTacToeCore)

# Add windows SDKs
target_include_directories(TicTacToeWin32 PUBLIC $ENV{UniversalCRTSdkDir}/Include/ucrt)
target_include_directories(TicTacToeWin32 PUBLIC $ENV{UniversalCRTSdkDir}/Include/10.0.10056.0/ucrt)

# Link core library
target_include_directories(TicTacToeWin32 PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/core/src")

# TODO: Add tests and install targets if needed.
