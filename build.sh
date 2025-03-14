#!/bin/sh
CC="bin/zig/zig-windows-x86_64-0.13.0/zig.exe cc"
INPUT="main.c external/raylib/lib/libraylib.a  external/kiss_fft/kiss_fft.c"
FLAGS="-Wall -Wextra -g -ggdb -Wno-unused-function -Wno-unused-parameter"
I_PATH="-I ./src -I./external/kiss_fft -I ./external/raylib/include -I ./src/interfaces/ -I ./src/ds/ -I ./src/core/ -I ./src/macros -I ./src/core/Andi/ -I ./src/core/Farras/ -I ./src/core/Mawl/ -I ./src/core/Nabil/ -I ./src/core/Paneji/ -I ./src/core/Zainandhi/"
LIBS="-lopengl32 -lgdi32 -lwinmm"
OUT="-o out/main.exe"

# Loop through src/**/* to add .c files to INPUT and .h files to I_PATH
for f in $(find src -name '*.c'); do
    INPUT="$INPUT $f"
done

echo "Generating compile_json"

rm -f compile_json_1 compile_commands.json
touch compile_json_1 compile_commands.json

$CC -MJ compile_json_1 $FLAGS $OUT $I_PATH $INPUT $L_PATH $LIBS
sleep 1
echo "[" > compile_commands.json
cat compile_json_1 >> compile_commands.json
echo "{}" >> compile_commands.json
echo "]" >> compile_commands.json
rm compile_json_1

echo "Compiling..."

echo $CC $FLAGS $OUT $I_PATH $INPUT $L_PATH $LIBS

time $CC $FLAGS $OUT $I_PATH $INPUT $L_PATH $LIBS
# $CC -E $FLAGS $I_PATH $INPUT $L_PATH $LIBS >> expand.c
if [ $? -ne 0 ]; then
    echo "Compilation failed. Exiting."
    exit $?
fi

echo "Compile completed successfully."
echo "Running the binary..."
out/main.exe
