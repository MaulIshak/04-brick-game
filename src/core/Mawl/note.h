#pragma once
#include "context.h"
#include "scene.h"
#include "raylib.h"
#include "timer.h"
#include "gameplay.h"

#ifndef NOTE_H
#define NOTE_H
#define NOTE_TEXTURE_COUNT 4

typedef struct {
  Gameplay *gp;
  AppContext *ctx;
  Beatmap beatmap;
  Texture2D noteTexture[NOTE_TEXTURE_COUNT];
  bool isBeatmapLoaded;
  float noteToPad;
  float timeToHitPad;
  Timer timer;
}DrawableNote;

void note_draw(DrawableNote *);
void note_update(DrawableNote *);
bool note_isShow(DrawableNote *);
void InitNote(DrawableNote*, AppContext*, Gameplay *);
void _drawBeatmapNote(DrawableNote*,Note);

impl_scene(DrawableNote*, Note_toScene, note_draw, note_update, note_isShow);
#endif // NOTE_H