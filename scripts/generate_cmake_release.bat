:: This script is used to generate the executable file for the project
:: Auteur : Noah Blattner

@ECHO OFF

set PATH=%PATH%;C:\Qt\Tools\mingw1120_64\bin

cd ..

:: We create the build folder if it doesn't exist
if not exist build mkdir build

:: We go to the build folder
cd build

:: We generate the exe with cmake
C:\Qt\Tools\CMake_64\bin\cmake.exe -G "MinGW Makefiles" .. -DCMAKE_BUILD_TYPE=Release

:: We compile the exe
C:\Qt\Tools\CMake_64\bin\cmake.exe --build . --config Release

