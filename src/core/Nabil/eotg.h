#pragma once
#include <raylib.h>
#include "scene.h"
#include "context.h"

#ifndef END_OF_THE_GAME_H
#define END_OF_THE_GAME_H

typedef struct EndOfTheGame {
    AppContext *ctx;
    Font gradeFont;
} EndOfTheGame;

void EndOfTheGame_Init(EndOfTheGame *self);
void EndOfTheGame_Draw(EndOfTheGame *self);
void EndOfTheGame_Update(EndOfTheGame *self);
bool EndOfTheGame_IsShow(EndOfTheGame *self);

impl_scene(EndOfTheGame*, EndOfTheGame_ToScene, EndOfTheGame_Draw, EndOfTheGame_Update, EndOfTheGame_IsShow )

#endif // END_OF_THE_GAME_H