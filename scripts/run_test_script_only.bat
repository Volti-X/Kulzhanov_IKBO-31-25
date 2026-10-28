@echo off
cd /d "%~dp0.."

echo ========================================
echo SCRIPT ONLY (NO VFS)
echo ========================================
echo.
"x64\Debug\shell_emulator.exe" --script tests/test2.txt
echo.

pause