@echo off
title Pharmacy Management System - Desktop Launcher
color 0A

echo.
echo ========================================
echo   PHARMACY MANAGEMENT SYSTEM
echo   DESKTOP LAUNCHER
echo ========================================
echo.
echo Creating desktop shortcut...
echo.

REM Get the current directory
set "CURRENT_DIR=%~dp0"
set "CURRENT_DIR=%CURRENT_DIR:~0,-1%"

REM Create a VBS script to create desktop shortcut
echo Set oWS = WScript.CreateObject("WScript.Shell") > CreateShortcut.vbs
echo sLinkFile = "%USERPROFILE%\Desktop\Pharmacy System.lnk" >> CreateShortcut.vbs
echo Set oLink = oWS.CreateShortcut(sLinkFile) >> CreateShortcut.vbs
echo oLink.TargetPath = "%CURRENT_DIR%\pharmacy_standalone.py" >> CreateShortcut.vbs
echo oLink.WorkingDirectory = "%CURRENT_DIR%" >> CreateShortcut.vbs
echo oLink.Description = "Pharmacy Management System" >> CreateShortcut.vbs
echo oLink.Save >> CreateShortcut.vbs

REM Run the VBS script
cscript CreateShortcut.vbs

REM Clean up
del CreateShortcut.vbs

echo.
echo Desktop shortcut created successfully!
echo.
echo You can now:
echo 1. Double-click the "Pharmacy System" shortcut on your desktop
echo 2. The system will start independently of Cursor AI
echo 3. You can close Cursor AI safely
echo.
echo Starting the system now...
echo.

start "Pharmacy System" python pharmacy_standalone.py

echo.
echo System started! You can now close Cursor AI.
echo.
pause

