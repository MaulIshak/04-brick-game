#pragma once
#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#define LINE_COUNT 4

#include "scene.h"
#include "raylib.h"
#include "context.h"
#include "timer.h"
#include "progress_bar.h"

#define LEFT_INDEX 0
#define UP_INDEX 1
#define RIGHT_INDEX 2
#define DOWN_INDEX 3

typedef struct LifeBar{
  Rectangle outlineRec;
  Rectangle lifeRec; 
  float width;
  float height;
} LifeBar;

typedef struct Gameplay{
  // ref ke Context aplikasi
  AppContext *ctx;
  // Lebar tampilan gameplay
  int width;
  // Texture untuk diload (load dari texturePath)
  // Texture untuk diload sekarang
  Texture2D textureToLoad[LINE_COUNT];
  // Texture pad active
  Texture2D padActiveTextureToLoad[LINE_COUNT];
  Texture2D padPassiveTextureToLoad[LINE_COUNT];
  // Background
  Texture2D background[3];
  // Path dari texture untuk diload
  // Texture pad biasa
  char *padPassiveTexturePaths [LINE_COUNT];
  char *padActiveTexturePaths [LINE_COUNT];
  // Opacity dari setiap pad (untuk mengaktifkan)
  int padActiveOpacity[LINE_COUNT];
  // Posisi setiap pad
  Vector2 padPositions[LINE_COUNT];
  // Waktu permainan dimulai dalam milisecond
  double gameTime;
  // Waktu game dimulaio
  double startGameTime;
  // Offset Waktu
  double gameTimeOffset;
  // Timer gameplay
  Timer timer;
  // Progress Bar
  ProgressBar progressBar;
  // Ukuran pad
  double padSize;
  // Alpha untuk flash effect dan akurasi
  double alpha;
  // Boolean agar tahu apakah background sudah di load
  bool isBackgroundLoaded;
  // Health untuk gameplay
  float life;
  // max Health
  float maxLife;
  // Life Bar
  LifeBar lifeBar;
}Gameplay;

#include "score.h"

// Draw layar gameplay
void gp_draw(Gameplay* self);

// Update gameplay
void gp_update(Gameplay* self);

// Menentukan apakah layar gameplay ditampilkan
bool gp_isShow(Gameplay* self);

// Inisialisasi gameplay, termasuk load texture
void InitGameplay(Gameplay *gameplay, AppContext *ctx);

// Modul untuk update life sesuai akurasi (dipanggil di note.c)
void UpdateLife(Gameplay* self,  Accuracy acc);

/*
================================================
  * Private
  * Modul yang tidak dipanggil di luar gameplay.c
================================================
*/

// Modul untuk load texture
void _LoadNoteTexture(Gameplay *self);

// Modul untuk update waktu permainan
void _UpdateGameTime(Gameplay* self);

// Modul untuk debug, untuk melihat zona akurasi
void _drawAccZone(Gameplay* self);

// Dapatkan Random Background dari 3 background yang ada
Texture2D _getRandomBg(Gameplay* self);

// Modul untuk menampilkan life bar ke layar
void _drawLifeBar(Gameplay* self);

// Modul untuk update lifebar;
void _updateLifeBar(Gameplay* self);

// modul untuk memberikan feedback pad active ketika input (D, F, J, K)
void _padFeedbackHandler(Gameplay* self);

impl_scene(Gameplay*, Gameplay_ToScene,gp_draw,gp_update, gp_isShow);

#endif  //GAMEPLAY_H