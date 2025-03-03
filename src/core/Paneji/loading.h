#pragma once
#include <raylib.h>
#include "scene.h"
#include "context.h"

#ifndef LOADING_H
#define LOADING_H

typedef struct Loading {
    AppContext *ctx;
} Loading;

void Loading_Draw(Loading *self);
void Loading_Update(Loading *self);
bool Loading_IsShow(Loading *self);

impl_scene(Loading*, Loading_ToScene, Loading_Draw, Loading_Update, Loading_IsShow)

#endif // LOADING_H