@echo off
:: Set the URL and destination directory

set zigUrl=https://ziglang.org/download/0.13.0/zig-windows-x86_64-0.13.0.zip
set destinationPath=external\zig
set zipFileName=zig-windows.zip

:: Create the destination directory if it doesn't exist
if not exist "%destinationPath%" (
    mkdir "%destinationPath%"
)

:: Download the Zig ZIP file using PowerShell
echo Downloading Zig from %zigUrl%...
powershell -Command "Invoke-WebRequest -Uri '%zigUrl%' -OutFile '%zipFileName%'"

:: Extract the ZIP file to the destination path
echo Extracting ZIP file to %destinationPath%...
powershell -Command "Expand-Archive -Path '%zipFileName%' -DestinationPath '%destinationPath%'"

:: Clean up by removing the ZIP file after extraction
echo Removing ZIP file...
del "%zipFileName%"

echo Zig has been installed and extracted to %destinationPath%.
pause
