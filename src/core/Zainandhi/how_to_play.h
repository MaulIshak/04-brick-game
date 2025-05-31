#include "scene.h"
#include "context.h"
#include "gameplay.h"
#ifndef HOW_TO_PLAY_H
#define HOW_TO_PLAY_H

static const char *imagePaths[] = {
    "resources/texture/how_to_play(1).png",
    "resources/texture/how_to_play(2).png",
    "resources/texture/how_to_play(3).png",
    "resources/texture/how_to_play(4).png",
    "resources/texture/how_to_play(5).png",
    "resources/texture/how_to_play(6).png",
    "resources/texture/how_to_play(7).png",
    "resources/texture/how_to_play(8).png",
};

#define NUM_PAGES (sizeof(imagePaths) / sizeof(imagePaths[0]))

typedef struct ImageNode {
    Texture2D texture;
    struct ImageNode *next;
    struct ImageNode *prev;
} ImageNode;

typedef struct HowToPlayManager {
    AppContext *ctx;
    Texture2D pages[NUM_PAGES];
    // int currentIndex;
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