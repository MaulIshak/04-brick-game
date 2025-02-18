#include "context.h"
#include "scene.h"
#include "raylib.h"

#ifndef NOTE_H
#define NOTE_H

typedef struct {
  Gameplay *gp;
  AppContext *ctx;
  Note *note;
  Vector2 position;
  float scale;
  float speed;
}DrawableNote;

void note_draw(DrawableNote *Note);
void note_update(DrawableNote *Note);
bool note_isShow(DrawableNote *Note);

impl_scene(DrawableNote*, Note_toScene, note_draw, note_update, note_isShow);
#endif // NOTE_H