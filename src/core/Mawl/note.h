#include "context.h"
#include "scene.h"
#include "raylib.h"
#include "timer.h"
#include "gameplay.h"
#include "score.h"

#ifndef NOTE_H
#define NOTE_H
#define NOTE_TEXTURE_COUNT 4
// #include "score.h"

// typedef enum Accuracy{
//   PERFECT,
//   GOOD,
//   MISS
// } Accuracy;

typedef struct{
    NoteDirection direction;
    double hit_at_ms;
    double duration_in_ms;
    Vector2 position;
    bool isSpawned;
    bool isHit;
    bool isHolding; 
    bool isHoldSuccess;
    bool isTrailVisible;
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
  // Note untuk ditampilkan
  NodeAddress noteHead;

  // Enum Akurasi akurasi note yang dihit
  Accuracy acc;

  ScoreManager *scoreManager;

  int  missCombo;
}NoteManager;

// Public
// Modul yang dipanggil di main

// Modul untuk menggambar tampilan note
void note_draw(NoteManager *);

// Modul untuk mengupdate variable variable yang dibutuhkan
void note_update(NoteManager *);

// Modul untuk menentukan kapan note digambar atau tampil
bool note_isShow(NoteManager *);

// Modul untuk inisiasi NoteManager
void InitNote(NoteManager*, AppContext*, Gameplay *, ScoreManager*);

// Private
// Modul untuk menampilkan Note dari beatmap
void _drawBeatmapNote(NoteManager*,DrawableNote);

// Modul untuk mengecek apakah note ditekan atau tidak
bool _isNoteHit(NoteManager*, DrawableNote);

// Modul untuk menampilkan akurasi
void _drawAccuracy(NoteManager*);

// Modul untuk update posisi note 
void _updateNotePosition(NoteManager*);

// Modul untuk meng-handle logic note ditekan beserta akurasinya
void _noteHitHandler(NoteManager*, DrawableNote*);

// Modul untuk extract note dari beatmap
void _extractNoteFromBeatmap(NoteManager*);

// modul untuk reset NoteManager
void _resetNoteManager(NoteManager*);

// modul untuk menggambar trail dari note hold
void _drawNoteTrail(NoteManager*, DrawableNote);

void _noteHoldHitHandler(NoteManager*, DrawableNote*);

// Implement Interface
impl_scene(NoteManager*, Note_toScene, note_draw, note_update, note_isShow);

#endif // NOTE_H