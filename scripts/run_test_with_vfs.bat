@echo off
cd /d "%~dp0.."

echo ========================================
echo WITH VFS AND SCRIPT
echo ========================================
echo.
"x64\Debug\shell_emulator.exe" --vfs data/minimal.json --script scripts/test1.txt
echo.

pause