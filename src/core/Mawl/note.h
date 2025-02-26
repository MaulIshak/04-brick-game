#pragma once
#include "context.h"
#include "scene.h"
#include "raylib.h"
#include "timer.h"
#include "gameplay.h"

#ifndef NOTE_H
#define NOTE_H
#define NOTE_TEXTURE_COUNT 4

typedef enum Accuracy{
  PERFECT,
  GOOD,
  MISS
} Accuracy;

typedef struct {
  // Ref ke gameplay
  Gameplay *gp;
  // Ref ke AppContext
  AppContext *ctx;
  // Beatmap untuk ditampilkan
  Beatmap beatmap;
  // Texture setiap note untuk diload
  Texture2D noteTexture[NOTE_TEXTURE_COUNT];
  // Penanda apakah beatmap sudah diload atau belum
  bool isBeatmapLoaded;
  // Jarak note ke pad
  float noteToPad;
  // Waktu untuk hit pad (speed);
  float timeToHitPad;
  // Timer
  Timer timer;
  // Penanda apakah musik sudah dimainkan atau belum
  bool isTrackPlayed;
  // Penanda apakah note pertama sudah hit/miss atau belum
  bool isFirstHit;
  // Enum akurasi untuk menandakan akurasi note yang dihit
  Accuracy acc;
}DrawableNote;

// Public
void note_draw(DrawableNote *);
void note_update(DrawableNote *);
bool note_isShow(DrawableNote *);
void InitNote(DrawableNote*, AppContext*, Gameplay *);

// Private
void _drawBeatmapNote(DrawableNote*,Note);
bool _isNoteHit(DrawableNote*, Note);
void _drawAccuracy(DrawableNote*);
void _updateNotePosition(DrawableNote*);

// Implement Interface
impl_scene(DrawableNote*, Note_toScene, note_draw, note_update, note_isShow);

#endif // NOTE_H