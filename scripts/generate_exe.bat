:: This script is used to generate the executable file for the project

@ECHO OFF

cd ..

:: We create the build folder if it doesn't exist
if not exist build mkdir build

:: We go to the build folder
cd build

:: We generate the exe with cmake
C:\Qt\Tools\CMake_64\bin\cmake.exe -G "MinGW Makefiles" ..
C:\Qt\Tools\CMake_64\bin\cmake.exe --build ..

:: We install the exe in the install folder
C:\Qt\Tools\CMake_64\bin\cmake.exe --install . --prefix .. --config Release
