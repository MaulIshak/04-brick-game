set CC=external\zig\zig-windows-x86_64-0.14.0-dev.3091+42e48b83b\zig.exe cc
set INPUT=main.c external/raylib/lib/libraylib.a
set FLAGS=-Wall -Wextra -g
set I_PATH=-I./src -I./external/raylib/include
set LIBS=-lopengl32 -lgdi32 -lwinmm

echo "Compiling..."

echo %CC%  %FLAGS% -o out\main.exe %I_PATH% %INPUT% %L_PATH% %LIBS% 

%CC%  %FLAGS% -o out\main.exe %I_PATH% %INPUT% %L_PATH% %LIBS%
echo "Compile Completed"

if %ERRORLEVEL% neq 0 (
    echo "Compilation failed. Exiting."
    exit /b %ERRORLEVEL%
)

echo "Compile completed successfully."
echo "Running the binary..."
out\main.exe