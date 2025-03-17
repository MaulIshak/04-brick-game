#ifndef ANIMASI_H
#define ANIMASI_H

#include "scene.h"
#include "context.h"
#include "raylib.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 800
#define TEXT_SPEED 4

typedef enum {
    LOGO_FADE_IN,
    LOGO_HOLD,
    LOGO_FADE_OUT,
    LOADING,
    READY
} GameState;

typedef struct Loading {
    AppContext *ctx;
    Texture2D logo;
    float alpha;
    float timer;
    bool loadingVisible;
    GameState state;
} Loading;



void LoadingLoadTextures(Loading *self);
void LoadingUnloadTextures(Loading *self);
void LoadingInitPositions(Loading *self);
void LoadingUpdatePositions(Loading *self);
void LoadingDrawTextures(Loading *self);
bool LoadingIsShow(Loading *self);

impl_scene(Loading*, Loading_ToScene, LoadingDrawTextures, LoadingUpdatePositions, LoadingIsShow )

#endif
