#include "progress_bar.h"
#include "gameplay.h"
#include "raylib.h"
#include "macro.h"
#include <stdio.h>

void InitProgressBar(ProgressBar *bar, float x, float y, float width, float height, Color color) {
    bar->x = x;
    bar->y = y;
    bar->width = width;
    bar->height = height;
    bar->color = color;
    bar->progress = 0.0f;
}

void UpdateProgressBar(ProgressBar *bar, Gameplay *gameplay) {
    if (gameplay->ctx->is_music_playing) {
        float totalLength = GetSelectedMusicLength(gameplay->ctx);
        float timePlayed = GetSelectedMusicTimePlayed(gameplay->ctx);
        
        if (totalLength > 0) {
            bar->progress = timePlayed / totalLength;
        }

        if (bar->progress > 1.0f) {
            bar->progress = 1.0f;
        }
    }
}

void DrawProgressBar(ProgressBar *bar) {
    DrawRectangle(bar->x, bar->y, bar->width, bar->height, LIGHTGRAY);
    DrawRectangleGradientH(bar->x, bar->y, bar->width * bar->progress, bar->height, SECONDARY_COLOR, TERTIARY_COLOR);

}
