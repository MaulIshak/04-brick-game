
set CC=bin\zig\zig-windows-x86_64-0.13.0\zig.exe cc
set INPUT=main.c external/raylib/lib/libraylib.a
set FLAGS=-Wall -Wextra -g
set I_PATH=-I ./src -I ./external/raylib/include
set LIBS=-lopengl32 -lgdi32 -lwinmm
set OUT=-o out\main.exe

echo "Generating compile_json"

%CC% -MJ compile_json_1 %FLAGS% %OUT% %I_PATH% %INPUT% %L_PATH% %LIBS% 

echo [ > compile_commands.json
cat compile_json_1 >> compile_commands.json
echo {} >> compile_commands.json 
echo ] >> compile_commands.json
del compile_json_1

echo "Compiling..."

echo %CC%  %FLAGS% %OUT% %I_PATH% %INPUT% %L_PATH% %LIBS% 

%CC%  %FLAGS% %OUT% %I_PATH% %INPUT% %L_PATH% %LIBS%


echo "Compile Completed"

if %ERRORLEVEL% neq 0 (
    echo "Compilation failed. Exiting."
    exit /b %ERRORLEVEL%
)

echo "Compile completed successfully."
echo "Running the binary..."
out\main.exe