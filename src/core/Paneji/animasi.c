#include "context.h"
#include "animasi.h"

void LoadingLoadTextures(Loading *self) {
    for (int i = 0; i < 5; i++) {
        self->texture[0] = LoadTexture("resources/texture/Arrow-1.png");
        self->texture[1] = LoadTexture("resources/texture/Arrow-2.png");
        self->texture[2] = LoadTexture("resources/texture/Arrow-3.png");
        self->texture[3] = LoadTexture("resources/texture/Arrow.png");
        self->texture[4] = LoadTexture("resources/texture/LOGO.png");  
    }
}

void LoadingUnloadTextures(Loading *self) {
    for (int i = 0; i < 5; i++) {
        UnloadTexture(self->texture[i]);
    }
}

void LoadingInitPositions(Loading *self) {
    int spacingX = SCREEN_WIDTH / 4;
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;
    self->FadeIn = 255.0f;
}

void LoadingUpdatePositions(Loading *self) {
    float FadeInSpeed = 2.5f;
    static int fadeDirection = -1;
    static bool isFadeActive = true;
    static float fadeTimer = 0.0f;

    if (!isFadeActive) return; // Jika animasi selesai, keluar

    if (fadeTimer > 0) {
        fadeTimer--;
        return; // Jangan lanjutkan perubahan fade selama delay
    }

    self->FadeIn += fadeDirection * FadeInSpeed;

    // Cek apakah mencapai batas fade in / out
    if (self->FadeIn <= 0) {
        self->FadeIn = 0;
        fadeDirection = 1;
        fadeTimer = 60;  // Delay sebelum mulai fade in
    } 
    else if (self->FadeIn >= 255) {
        self->FadeIn = 255;
        fadeDirection = -1;
        
        // **Hentikan fade setelah kembali ke nilai awal**
        isFadeActive = false;
    }
}

void LoadingDrawTextures(Loading *self) {

    int centerXTex = SCREEN_WIDTH / 2 - self->texture[4].width / 2 - 120;
    int centerYTex = SCREEN_HEIGHT / 2 - self->texture[4].height / 2 - 120;

    DrawRectangle(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,(Color){0,0,0,(unsigned char)self->FadeIn});
    DrawTextureEx(self->texture[4], (Vector2){ centerXTex, centerYTex }, 0.0f, 1.5f, (Color){255,255,255,255});
}

bool LoadingIsShow(Loading *self) {
    return self->ctx->app_state == APP_LOADING;
}

