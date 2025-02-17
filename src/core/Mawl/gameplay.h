#include "scene.h"
#include "raylib.h"
#include "context.h"

#ifndef GAMEPLAY_H
#define GAMEPLAY_H

typedef struct Gameplay{
  AppContext *ctx;
  int width;
}Gameplay;

void gp_draw(Gameplay* self);
void gp_update(Gameplay* self);
bool gp_isShow(Gameplay* self);

impl_scene(Gameplay*, gameplay_ToScene,gp_draw,gp_update, gp_isShow);

#endif  //GAMEPLAY_H