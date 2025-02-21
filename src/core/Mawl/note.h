#include "context.h"
#include "scene.h"
#include "raylib.h"

#ifndef NOTE_H
#define NOTE_H
#define NOTE_TEXTURE_COUNT 4

typedef struct {
  // Gameplay *gp;
  AppContext *ctx;
  Beatmap beatmap;
  Texture2D noteTexture[NOTE_TEXTURE_COUNT];
  bool isBeatmapLoaded;
}DrawableNote;

void note_draw(DrawableNote *);
void note_update(DrawableNote *);
bool note_isShow(DrawableNote *);
void InitNote(DrawableNote *, AppContext*);

impl_scene(DrawableNote*, Note_toScene, note_draw, note_update, note_isShow);
#endif // NOTE_H