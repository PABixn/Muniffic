@echo off
cmake -P VulkanSDK.cmake > output.txt
for /f "tokens=*" %%i in ('findstr "VULKAN_SDK_LOCATION is set to:" output.txt') do (
    set "line=%%i"
)

for /f "tokens=6" %%a in ("%line%") do (
    set "VULKAN_SDK_LOCATION=%%a"
)
echo %VULKAN_SDK_LOCATION%
for /D %%d in ("*") do (
    pushd %%d
    cd %%d
    %VULKAN_SDK_LOCATION%\Bin\glslc.exe shader.frag -o frag.spv
    %VULKAN_SDK_LOCATION%\Bin\glslc.exe shader.vert -o vert.spv
    cd ../
    popd
)
del output.txt

pause