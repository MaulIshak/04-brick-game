#pragma once
#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#define TEXTURE_COUNT 4
#define LINE_COUNT 4

#include "scene.h"
#include "raylib.h"
#include "context.h"
#include "timer.h"
#include "progress_bar.h"

typedef struct Gameplay{
  // Context aplikasi
  AppContext *ctx;
  // Lebar tampilan gameplay
  int width;
  // Texture untuk diload
  Texture2D textureToLoad[TEXTURE_COUNT];
  Texture2D activeTextureToLoad[TEXTURE_COUNT];
  Texture2D passiveTextureToLoad[TEXTURE_COUNT];
  //
  Texture2D background[3];
  // Path dari texture untuk diload
  char *texturePaths [TEXTURE_COUNT];
  char *textureActivePaths [TEXTURE_COUNT];
  // Opacity dari setiap pad
  int padOpacity[LINE_COUNT];
  // Positi setiap pad
  Vector2 padPositions[LINE_COUNT];
  // Waktu permainan dimulai dalam milisecond
  double gameTime;
  // Timer
  double startGameTime;
  // Offset Waktu
  double gameTimeOffset;
  // Timer
  Timer timer;
  // Progress Bar
  ProgressBar progressBar;
  // Ukuran pad
  double padSize;
  // Alpha untuk flash effect dan akurasi
  double alpha;

  // Boolean agar tahu apakah background sudah di load
  bool isBackgroundLoaded;
}Gameplay;

// Draw layar gameplay
void gp_draw(Gameplay* self);

// Update gameplay
void gp_update(Gameplay* self);

// Menentukan apakah layar gameplay ditampilkan
bool gp_isShow(Gameplay* self);

// Inisialisasi gameplay, termasuk load texture
void InitGameplay(Gameplay *gameplay, AppContext *ctx);

// Private procedure
// Modul untuk load texture
void _LoadNoteTexture(Gameplay *self);

// Modul untuk update waktu permainan
void _UpdateGameTime(Gameplay* self);

// Modul untuk debug, untuk melihat zona akurasi
void _drawAccZone(Gameplay* self);

// Dapatkan Random Background dari 3 background yang ada
Texture2D _getRandomBg(Gameplay* self);

impl_scene(Gameplay*, Gameplay_ToScene,gp_draw,gp_update, gp_isShow);

#endif  //GAMEPLAY_H