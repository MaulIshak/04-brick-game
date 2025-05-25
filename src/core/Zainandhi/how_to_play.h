#include "scene.h"
#include "context.h"
#include "gameplay.h"
#ifndef HOW_TO_PLAY_H
#define HOW_TO_PLAY_H

typedef struct ImageNode {
    Texture2D texture;
    struct ImageNode *next;
    struct ImageNode *prev;
} ImageNode;

typedef struct HowToPlayManager {
    AppContext *ctx;
    ImageNode *head;
    ImageNode *current;
    bool isLoaded;
} HowToPlayManager;

HowToPlayManager InitHowToPlay(AppContext *ctx);

void UpdateHowToPlay(HowToPlayManager *how);

void DrawHowToPlay(HowToPlayManager *how);

bool IsShowHowToPlay(HowToPlayManager *how);

void UnloadHowToPlayImages(HowToPlayManager *how);

impl_scene(HowToPlayManager *, HowToPlay_ToScene, DrawHowToPlay, UpdateHowToPlay, IsShowHowToPlay);

#endif