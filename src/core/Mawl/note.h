#include "context.h"
#include "scene.h"
#include "raylib.h"
// #include "gameplay.h"

#ifndef NOTE_H
#define NOTE_H

typedef struct {
  // Gameplay *gp;
  AppContext *ctx;
  Note *note;
  Vector2 position;
  float scale;
  float speed;
}DrawableNote;

void note_draw(DrawableNote *);
void note_update(DrawableNote *);
bool note_isShow(DrawableNote *);

impl_scene(DrawableNote*, Note_toScene, note_draw, note_update, note_isShow);
#endif // NOTE_H