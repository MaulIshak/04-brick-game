#!/bin/sh
CC="gcc"
INPUT="main.c external/raylib-linux/lib/libraylib.a external/kiss_fft/kiss_fft.c"
FLAGS="-Wall -Wextra -g -ggdb -Wno-unused-function -Wno-unused-parameter"
I_PATH="-I ./src -I./external/kiss_fft  -I ./external/raylib-linux/include -I ./src/interfaces/ -I ./src/ds/ -I ./src/core/ -I ./src/macros -I ./src/core/Andi/ -I ./src/core/Farras/ -I ./src/core/Mawl/ -I ./src/core/Nabil/ -I ./src/core/Paneji/ -I ./src/core/Zainandhi/"
LIBS="-lGL -lm -lpthread -ldl -lrt -lX11"
OUT="-o out/main.exe"

# Loop through src/**/* to add .c files to INPUT and .h files to I_PATH
for f in $(find src -name '*.c'); do
    INPUT="$INPUT $f"
done

echo "Compiling..."

echo $CC $FLAGS $OUT $I_PATH $INPUT $L_PATH $LIBS

bear -- $CC $FLAGS $OUT $I_PATH $INPUT $L_PATH $LIBS
# $CC -E $FLAGS $I_PATH $INPUT $L_PATH $LIBS >> expand.c
if [ $? -ne 0 ]; then
    echo "Compilation failed. Exiting."
    exit $?
fi

echo "Compile completed successfully."
echo "Running the binary..."
out/main.exe
