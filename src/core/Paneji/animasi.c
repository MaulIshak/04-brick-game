#include "context.h"
#include "animasi.h"

void LoadingLoadTextures(Loading *self) {
    for (int i = 0; i < 4; i++) {
        self->texture[i] = LoadTexture("resources/texture/lambang-contoh.png");  
    }
}

void LoadingUnloadTextures(Loading *self) {
    for (int i = 0; i < 4; i++) {
        UnloadTexture(self->texture[i]);
    }
}

void LoadingInitPositions(Loading *self) {
    int spacingX = SCREEN_WIDTH / 4;
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;

    self->positions[0] = (Vector2){ spacingX - 250, centerY - 270 };
    self->positions[1] = (Vector2){ SCREEN_WIDTH + 50, centerY - 270 };
    self->positions[2] = (Vector2){ spacingX - 350, centerY + 70 };
    self->positions[3] = (Vector2){ SCREEN_WIDTH + 100, centerY + 70 };
}

void LoadingUpdatePositions(Loading *self) {
    int leftStop = SCREEN_WIDTH / 3 - 100;
    int rightStop = 2 * SCREEN_WIDTH / 3;
    static float verticalOffsets[4] = {0};
    static int direction[4] = {1, -1, 1, -1};
    static float speeds[4] = {0.07f, 0.10f, 0.12f, 0.13f}; // Kecepatan berbeda tiap tekstur

    for (int i = 0; i < 4; i++) {
        if (i % 2 == 0) { // Grup kiri
            if (self->positions[i].x < leftStop) {
                self->positions[i].x += TEXT_SPEED;
                if (self->positions[i].x > leftStop) self->positions[i].x = leftStop;
            }
        } else { // Grup kanan
            if (self->positions[i].x > rightStop) {
                self->positions[i].x -= TEXT_SPEED;
                if (self->positions[i].x < rightStop) self->positions[i].x = rightStop;
            }
        }
        
        // Gerakan naik-turun langsung sejak awal
        verticalOffsets[i] += direction[i] * speeds[i];
        if (verticalOffsets[i] > 25 || verticalOffsets[i] < -25) {
            direction[i] *= -1;
        }
        self->positions[i].y += direction[i] * speeds[i];
    }
}

void LoadingDrawTextures(Loading *self) {
    for (int i = 0; i < 4; i++) {
        DrawTextureEx(self->texture[i], self->positions[i], 0.0f, 0.2f, WHITE);
    }
}

bool LoadingIsShow(Loading *self) {
    return self->ctx->app_state == APP_LOADING;
}
