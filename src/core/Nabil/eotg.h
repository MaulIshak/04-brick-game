#pragma once
#include <raylib.h>
#include "scene.h"
#include "context.h"

#ifndef END_OF_THE_GAME_H
#define END_OF_THE_GAME_H

// Local struct
typedef struct EndOfTheGame {
    AppContext *ctx;
    Font gradeFont;
    float anim_accuracy;
    long anim_point;
    float animation_timer;
    bool animation_done;
    bool count_played;
} EndOfTheGame;

// Modul Inisialisasi
void EndOfTheGame_Init(EndOfTheGame *self);
// Modul untuk tampilan
void EndOfTheGame_Draw(EndOfTheGame *self);
// Modul untuk logic
void EndOfTheGame_Update(EndOfTheGame *self);
// Modul untuk mengecek tampilkan atau tidak
bool EndOfTheGame_IsShow(EndOfTheGame *self);

// Menampilkan screen
impl_scene(EndOfTheGame*, EndOfTheGame_ToScene, EndOfTheGame_Draw, EndOfTheGame_Update, EndOfTheGame_IsShow )

#endif // END_OF_THE_GAME_H