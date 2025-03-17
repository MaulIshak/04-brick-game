#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include "raylib.h"
struct Gameplay;
typedef struct ProgressBar {
    float x, y;
    float width, height;
    float progress;
    Color color;
} ProgressBar;

void InitProgressBar(ProgressBar *bar, float x, float y, float width, float height, Color color);
void UpdateProgressBar(ProgressBar *bar, struct Gameplay *gameplay);
void DrawProgressBar(ProgressBar *bar);

#endif
