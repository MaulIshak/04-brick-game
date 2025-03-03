#ifndef ANIMASI_H
#define ANIMASI_H

#include "raylib.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 800
#define TEXT_SPEED 4

void LoadTextures(Texture2D textures[]);
void UnloadTextures(Texture2D textures[]);
void InitPositions(Vector2 positions[], Texture2D textures[]);
void UpdatePositions(Vector2 positions[], Texture2D textures[]);
void DrawTextures(Texture2D textures[], Vector2 positions[]);

#endif
