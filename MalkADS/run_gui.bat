@echo off
REM Quick start script for the GUI (uses development mode if C++ module not built)

echo Starting AUC Library System GUI...
echo.

python gui/main.py

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Failed to start GUI
    echo Make sure Python and dependencies are installed
    pause
)
