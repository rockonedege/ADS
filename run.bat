:: this script wraps CMake commands to generate build script for the following tool chains in to a build temp folder: ~temp
:: - NMake
:: - Visual Studio 2015 x32
:: - Visual Studio 2015 x64

@echo off

:: create build folder if non-existent
set BUILD_DIR=%~dp0\~temp
if not exist %BUILD_DIR%\NUL md %BUILD_DIR%
echo %BUILD_DIR%
:: create for VS2015 x32
cd %BUILD_DIR%
set VS2015_32_ROOT=%BUILD_DIR%\vs2015-x32
if not exist %VS2015_32_ROOT%\NUL md vs2015-x32
cd %VS2015_32_ROOT%
cmake ..\.. -G"Visual Studio 14 2015"
@echo done generating Visual Studio 14 2015 x32 into %VS2015_32_ROOT%

:: create for VS2015 x64
cd %BUILD_DIR%
set VS2015_64_ROOT=%BUILD_DIR%\vs2015-x64
if not exist %VS2015_64_ROOT%\NUL md vs2015-x64
cd %VS2015_64_ROOT%
cmake ..\.. -G"Visual Studio 14 2015 Win64"
@echo done generating Visual Studio 14 2015 Win64 into %VS2015_64_ROOT%

:: create for namke
cd %BUILD_DIR%
set NMAKE_ROOT=%BUILD_DIR%\nmake
if not exist %NMAKE_ROOT%\NUL md nmake
cd %NMAKE_ROOT%
cmake ..\.. -G"NMake Makefiles"
@echo done generating NMake Makefiles  into %NMAKE_ROOT%