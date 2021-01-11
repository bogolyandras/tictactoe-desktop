# CMakeList.txt : CMake project for winapi, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add windows SDKs
include_directories($ENV{UniversalCRTSdkDir}/Include/ucrt)
include_directories($ENV{UniversalCRTSdkDir}/Include/10.0.10056.0/ucrt)

# Add source to this project's executable.
add_executable (TicTacToe WIN32 "winentry.cpp")

# TODO: Add tests and install targets if needed.
