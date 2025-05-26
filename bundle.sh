CC="bin/zig/zig-windows-x86_64-0.13.0/zig.exe cc"
INPUT="main.c external/raylib/lib/libraylib.a  external/kiss_fft/kiss_fft.c resource.o  ./out/sqlite.o "
# FLAGS="-O2 -flto -Wall -Wextra -g -ggdb -Wno-unused-function -Wno-unused-parameter -mwindows -Wl,--subsystem,windows"
FLAGS="-Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -mwindows -Wl,--subsystem,windows"
I_PATH="-I ./src -I./external/kiss_fft -I./external -I./external/sqlite -I ./external/raylib/include -I ./src/interfaces/ -I ./src/ds/ -I ./src/core/ -I ./src/macros -I ./src/core/Andi/ -I ./src/core/Farras/ -I ./src/core/Mawl/ -I ./src/core/Nabil/ -I ./src/core/Paneji/ -I ./src/core/Zainandhi/"
LIBS="-lopengl32 -lgdi32 -lwinmm"
OUT="-o rythm_me/game.exe"

# Loop through src/**/* to add .c files to INPUT and .h files to I_PATH
for f in $(find src -name '*.c' ! -name '*.standalone.c'); do
    INPUT="$INPUT $f"
done

rm -r rythm_me/ ./rythm_me.zip
mkdir rythm_me
echo "Compiling..."
time $CC $FLAGS $OUT $I_PATH $INPUT $L_PATH $LIBS
cp -r ./resources/ ./rythm_me
cp ./beatmap.db ./rythm_me
zip -r -9 ./rythm_me.zip ./rythm_me