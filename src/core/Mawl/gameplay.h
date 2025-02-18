#include "scene.h"
#include "raylib.h"
#include "context.h"

#ifndef GAMEPLAY_H
#define GAMEPLAY_H

typedef struct Gameplay{
  AppContext *ctx;
  int width;
  bool isTextureLoaded;
  Texture2D textureToLoad;
  char* texturePath;
}Gameplay;

void gp_draw(Gameplay* self);
void gp_update(Gameplay* self);
bool gp_isShow(Gameplay* self);
void init_gameplay(Gameplay *gameplay,AppContext *ctx, int width);
void _loadTexture(Gameplay *self);

impl_scene(Gameplay*, Gameplay_ToScene,gp_draw,gp_update, gp_isShow);

#endif  //GAMEPLAY_H