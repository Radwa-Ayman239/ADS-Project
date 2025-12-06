@echo off
cls
echo ==========================================
echo       AUC Library System Launcher
echo ==========================================
echo.
echo Please select the mode you want to run:
echo.
echo [1] CLI (Command Line Interface)
echo [2] GUI (Graphical User Interface)
echo.

set /p choice="Enter your choice (1 or 2): "

if not exist "cmake-build-debug\MalkADS.exe" (
    echo.
    echo ERROR: Executable not found in cmake-build-debug/
    echo Please build the project first.
    pause
    exit /b
)

cd cmake-build-debug

if "%choice%"=="1" (
    echo.
    echo Starting CLI...
    echo.
    MalkADS.exe
) else if "%choice%"=="2" (
    echo.
    echo Starting GUI...
    echo.
    MalkADS.exe --gui
) else (
    echo.
    echo Invalid choice. Please run the script again and select 1 or 2.
    pause
    exit /b
)

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Application exited with error code %ERRORLEVEL%
    pause
)
