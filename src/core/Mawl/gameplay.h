#pragma once
#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#define LINE_COUNT 4

#include "scene.h"
#include "raylib.h"
#include "context.h"
#include "timer.h"
#include "progress_bar.h"


typedef struct LifeBar{
  Rectangle outlineRec;
  Rectangle lifeRec; 
  float width;
  float height;
} LifeBar;

typedef struct Gameplay{
  // Context aplikasi
  AppContext *ctx;
  // Lebar tampilan gameplay
  int width;
  // Texture untuk diload
  Texture2D textureToLoad[LINE_COUNT];
  Texture2D activeTextureToLoad[LINE_COUNT];
  Texture2D passiveTextureToLoad[LINE_COUNT];
  // Background
  Texture2D background[3];
  // Path dari texture untuk diload
  char *texturePaths [LINE_COUNT];
  char *textureActivePaths [LINE_COUNT];
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
  
  //Health untuk gameplay
  float life;
  // Maks Health
  float maxLife;
  // Life Bar
  LifeBar lifeBar;
  bool isPlaying;
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

void UpdateLife(Gameplay* self,  Accuracy acc);

// Private procedure
// Modul untuk load texture
void _LoadNoteTexture(Gameplay *self);

// Modul untuk update waktu permainan
void _UpdateGameTime(Gameplay* self);

// Modul untuk debug, untuk melihat zona akurasi
void _drawAccZone(Gameplay* self);

// Dapatkan Random Background dari 3 background yang ada
Texture2D _getRandomBg(Gameplay* self);

void _drawLifeBar(Gameplay* self);
void _updateLifeBar(Gameplay* self);
void _inputHandler(Gameplay* self);

impl_scene(Gameplay*, Gameplay_ToScene,gp_draw,gp_update, gp_isShow);

#endif  //GAMEPLAY_H