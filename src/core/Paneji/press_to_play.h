#pragma once
#include <raylib.h>
#include "scene.h"
#include "context.h"

#ifndef PRESS_TO_PLAY_H
#define PRESS_TO_PLAY_H

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 800
#define MENU_COUNT 3

typedef struct PressToPlay {
    AppContext *ctx;
    Texture2D logogame;
    int selectedIndex;
    const char *menuItems[MENU_COUNT];
    Color WarnaAwal[MENU_COUNT];
} PressToPlay;

void PressToPlay_LoadTextures(PressToPlay *self);
void PressToPlay_UnloadTextures(PressToPlay *self);
void PressToPlay_Draw(PressToPlay *self);
void PressToPlay_Update(PressToPlay *self);
bool PressToPlay_IsShow(PressToPlay *self);

impl_scene(PressToPlay*, PressToPlay_ToScene, PressToPlay_Draw, PressToPlay_Update, PressToPlay_IsShow )

#endif // PRESS_TO_PLAY_H
