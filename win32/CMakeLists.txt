# specify the C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED true)

# Add source to this project's executable.
add_executable (TicTacToeWin32 WIN32 "src/winmain.cxx" "src/win32util.cxx" "src/mainwindowlogic.cxx" "resource/resource.rc")
target_link_libraries(TicTacToeWin32 PUBLIC ComCtl32.lib d2d1.lib)
target_link_libraries(TicTacToeWin32 PUBLIC TicTacToeCore)

add_compile_definitions(UNICODE _UNICODE, WIN32_LEAN_AND_MEAN)

if(MINGW)
    target_compile_options(TicTacToeWin32 PRIVATE -municode)
    target_link_options(TicTacToeWin32 PRIVATE -municode)
endif()

# Add windows SDKs
target_include_directories(TicTacToeWin32 PUBLIC $ENV{UniversalCRTSdkDir}/Include/ucrt)
target_include_directories(TicTacToeWin32 PUBLIC $ENV{UniversalCRTSdkDir}/Include/10.0.10056.0/ucrt)

# Link core library
target_include_directories(TicTacToeWin32 PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/core/src")
