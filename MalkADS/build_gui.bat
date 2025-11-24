@echo off
REM Build script for AUC Library System with Python GUI

echo ========================================
echo AUC Library System - Build Script
echo ========================================
echo.

REM Check if build directory exists
if not exist "cmake-build-debug" (
    echo Creating build directory...
    mkdir cmake-build-debug
)

cd cmake-build-debug

echo Configuring CMake...
cmake ..

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: CMake configuration failed!
    echo Make sure CMake is installed and pybind11 is available.
    echo Try: pip install "pybind11[global]"
    pause
    exit /b 1
)

echo.
echo Building project...
cmake --build . --config Release

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Build failed!
    pause
    exit /b 1
)

cd ..

echo.
echo ========================================
echo Build completed successfully!
echo ========================================
echo.
echo The library_system module should now be available.
echo You can run the GUI with: python gui/main.py
echo.
pause
