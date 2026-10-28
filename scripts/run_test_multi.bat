@echo off
cd /d "%~dp0.."
echo ========================================
echo MULTIPLE FILES VFS TEST
echo ========================================
echo.
"x64\Debug\shell_emulator.exe" --vfs data/multiple_files.json
pause