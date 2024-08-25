@echo off
cd ../
cmake .
cd Engine-Editor\SandboxProject\Assets\Scripts
cmake --build .
pause