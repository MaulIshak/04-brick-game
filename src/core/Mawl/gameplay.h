#pragma once
#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#define TEXTURE_COUNT 4
#define LINE_COUNT 4

#include "scene.h"
#include "raylib.h"
#include "context.h"

typedef struct Gameplay{
  AppContext *ctx;
  int width;
  Texture2D textureToLoad[TEXTURE_COUNT];
  char *texturePaths [TEXTURE_COUNT];
  int padOpacity[LINE_COUNT];
  Vector2 padPositions[LINE_COUNT];
}Gameplay;


void gp_draw(Gameplay* self);
void gp_update(Gameplay* self);
bool gp_isShow(Gameplay* self);

// Inisialisasi gameplay, termasuk load texture
void InitGameplay(Gameplay *gameplay, AppContext *ctx);

// Private procedure untuk load texture
void _LoadNoteTexture(Gameplay *self);

impl_scene(Gameplay*, Gameplay_ToScene,gp_draw,gp_update, gp_isShow);

#endif  //GAMEPLAY_H