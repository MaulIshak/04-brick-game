#pragma once
#include "context.h"
#include "scene.h"

#ifndef BACKGROUND_H
#define BACKGROUND_H

typedef struct Background {
    AppContext* ctx;
} Background;

void Background_MusicCallback(void *bufferData, unsigned int frames);
Background CreateBackground(AppContext* ctx);
void Background_Draw(Background* self);
void Background_Update(Background* self);
bool Background_IsShow(Background* self);

impl_scene(Background*, Background_ToScene, Background_Draw, Background_Update, Background_IsShow)

#endif // BACKGROUND_H