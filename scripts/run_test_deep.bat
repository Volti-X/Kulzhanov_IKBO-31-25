@echo off
cd /d "%~dp0.."
echo ========================================
echo DEEP NESTED VFS TEST
echo ========================================
echo.
"x64\Debug\shell_emulator.exe" --vfs data/deep_nested.json --script tests/test_stage3.txt
pause