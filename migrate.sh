#!/bin/sh
CC="bin/zig/zig-windows-x86_64-0.13.0/zig.exe cc"
INPUT="src/core/Andi/migrations.standalone.c external/raylib/lib/libraylib.a ./external/sqlite/sqlite3.c external/kiss_fft/kiss_fft.c"
FLAGS="-g -ggdb -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -fsanitize=undefined -fsanitize-trap=undefined"
I_PATH="-D GAME_DEBUG=1 -I ./src -I./external/kiss_fft -I./external -I./external/sqlite -I ./external/raylib/include -I ./src/interfaces/ -I ./src/ds/ -I ./src/core/ -I ./src/macros -I ./src/core/Andi/ -I ./src/core/Farras/ -I ./src/core/Mawl/ -I ./src/core/Nabil/ -I ./src/core/Paneji/ -I ./src/core/Zainandhi/"
LIBS="-lopengl32 -lgdi32 -lwinmm"
OUT="-o out/migrate.exe"

# Loop through src/**/* to add .c files to INPUT and .h files to I_PATH
for f in $(find src -name '*.c' ! -name '*.standalone.c'); do
    INPUT="$INPUT $f"
done

time $CC $FLAGS $OUT $I_PATH $INPUT $L_PATH $LIBS
./out/migrate.exe