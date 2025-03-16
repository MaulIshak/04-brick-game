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
  // Path dari texture untuk diload
  char *texturePaths [TEXTURE_COUNT];
  // Opacity dari setiap pad
  int padOpacity[LINE_COUNT];
  // Positi setiap pad
  Vector2 padPositions[LINE_COUNT];
  // Waktu permainan dimulai dalam milisecond
  double gameTime;
  // Timer
  double startGameTime;
  // Offset
  double gameTimeOffset;
  Timer timer;
  // Progress Bar
  ProgressBar progressBar;
}Gameplay;


void gp_draw(Gameplay* self);
void gp_update(Gameplay* self);
bool gp_isShow(Gameplay* self);

// Inisialisasi gameplay, termasuk load texture
void InitGameplay(Gameplay *gameplay, AppContext *ctx);

// Private procedure
void _LoadNoteTexture(Gameplay *self);
void _UpdateGameTime(Gameplay* self);

impl_scene(Gameplay*, Gameplay_ToScene,gp_draw,gp_update, gp_isShow);

#endif  //GAMEPLAY_H