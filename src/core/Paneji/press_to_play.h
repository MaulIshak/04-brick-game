#pragma once
#include <raylib.h>
#include "scene.h"
#include "context.h"
#ifndef PRESS_TO_PLAY_H
#define PRESS_TO_PLAY_H

typedef struct PressToPlay {
    AppContext *ctx;
} PressToPlay;

void PressToPlay_Draw(PressToPlay *self);
void PressToPlay_Update(PressToPlay *self);
bool PressToPlay_IsShow(PressToPlay *self);

impl_scene(PressToPlay*, PressToPlay_ToScene, PressToPlay_Draw, PressToPlay_Update, PressToPlay_IsShow )

#endif // PRESS_TO_PLAY_H
