#!/bin/sh
CC="bin/zig/zig-windows-x86_64-0.13.0/zig.exe cc"
INPUT="main.c external/raylib/lib/libraylib.a  external/kiss_fft/kiss_fft.c"
FLAGS="-g -ggdb -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -fsanitize=undefined -fsanitize-trap=undefined"
I_PATH="-D GAME_DEBUG=1 -I ./src -I./external/kiss_fft -I ./external/raylib/include -I ./src/interfaces/ -I ./src/ds/ -I ./src/core/ -I ./src/macros -I ./src/core/Andi/ -I ./src/core/Farras/ -I ./src/core/Mawl/ -I ./src/core/Nabil/ -I ./src/core/Paneji/ -I ./src/core/Zainandhi/"
LIBS="-lopengl32 -lgdi32 -lwinmm"
OUT="-o out/main.exe"

# Loop through src/**/* to add .c files to INPUT and .h files to I_PATH
for f in $(find src -name '*.c'); do
    INPUT="$INPUT $f"
done

# echo "Generating compile_commands.json"

# Remove previous files
rm -f compile_commands.json

Run the compiler to generate compile_json_1
if ! $CC -MJ compile_commands.json $FLAGS $OUT $I_PATH $INPUT $LIBS; then
    echo "Compiler failed to generate compile_commands data. For whatever reason -_-."
fi
sleep 2

sed -i '1s/^/[\n/' compile_commands.json
echo "{}]" >> compile_commands.json

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
