#ifndef ANIMASI_H
#define ANIMASI_H

#include "scene.h"
#include "context.h"
#include "raylib.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 800
#define TEXT_SPEED 4

typedef struct Loading {
    AppContext *ctx;
    Texture2D texture[8];
    Vector2 positions[8];
} Loading;

void LoadingLoadTextures(Loading *self);
void LoadingUnloadTextures(Loading *self);
void LoadingInitPositions(Loading *self);
void LoadingUpdatePositions(Loading *self);
void LoadingDrawTextures(Loading *self);
bool LoadingIsShow(Loading *self);

impl_scene(Loading*, Loading_ToScene, LoadingDrawTextures, LoadingUpdatePositions, LoadingIsShow )

#endif
