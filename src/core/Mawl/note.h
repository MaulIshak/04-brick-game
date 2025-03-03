#include "context.h"
#include "scene.h"
#include "raylib.h"
#include "timer.h"
#include "gameplay.h"

#ifndef NOTE_H
#define NOTE_H
#define NOTE_TEXTURE_COUNT 4
// #include "score.h"

typedef enum Accuracy{
  PERFECT,
  GOOD,
  MISS
} Accuracy;

typedef struct{
  int perfectUpperOffset;
  int perfectLowerOffset;
  int goodUpperOffset;
  int goodLowerOffset;
  int missUpperOffset;
  int missLowerOffset;
} AccuracyOffset;

typedef struct{
    NoteDirection direction;
    double hit_at_ms;
    Vector2 position;
    bool isSpawned;
    bool isHit;
}DrawableNote;


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
  Timer musicTimer;
  // Penanda apakah musik sudah dimainkan atau belum
  bool isTrackPlayed;
  // Penanda apakah note pertama sudah hit/miss atau belum
  bool isFirstHit;
  // Enum akurasi untuk menandakan akurasi note yang dihit
  Accuracy acc;
  AccuracyOffset accOff;
  DrawableNote note [1024];
}NoteManager;

// Public
void note_draw(NoteManager *);
void note_update(NoteManager *);
bool note_isShow(NoteManager *);
void InitNote(NoteManager*, AppContext*, Gameplay *);

// Private
void _drawBeatmapNote(NoteManager*,DrawableNote);
bool _isNoteHit(NoteManager*, DrawableNote);
void _drawAccuracy(NoteManager*);
void _updateNotePosition(NoteManager*);
void _noteHitHandler(NoteManager*, DrawableNote*);
void _extractNoteFromBeatmap(NoteManager*);

// Implement Interface
impl_scene(NoteManager*, Note_toScene, note_draw, note_update, note_isShow);

#endif // NOTE_H