#include "context.h"
#include "animasi.h"

void LoadingLoadTextures(Loading *self) {
    self->logo = LoadTexture("resources/texture/lambang-contoh.png");  // Pastikan file logo ada
}

void LoadingUnloadTextures(Loading *self) {
    UnloadTexture(self->logo);
}

void LoadingInitPositions(Loading *self) {
    self->alpha = 0.0f;
    self->timer = 0.0f;
    self->loadingVisible = true;
    self->state = LOGO_FADE_IN;
}

void LoadingUpdatePositions(Loading *self) {
    if (self->state == LOGO_FADE_IN) {
        self->alpha += 0.02f;
        if (self->alpha >= 1.0f) {
            self->alpha = 1.0f;
            self->state = LOGO_HOLD;
            self->timer = 1.5f;
        }
    } 
    else if (self->state == LOGO_HOLD) {
        self->timer -= GetFrameTime();
        if (self->timer <= 0) {
            self->state = LOGO_FADE_OUT;
        }
    } 
    else if (self->state == LOGO_FADE_OUT) {
        self->alpha -= 0.02f;
        if (self->alpha <= 0.0f) {
            self->alpha = 0.0f;
            self->state = LOADING;
            self->timer = 0.0f;
        }
    } 
    else if (self->state == LOADING) {
        self->timer += GetFrameTime();
        if (self->timer >= 0.5f) {
            self->loadingVisible = !self->loadingVisible;
            self->timer = 0.0f;
        }
        if (self->timer >= 3.0f) {
            self->state = READY;
        }
    } 
    else if (self->state == READY || IsKeyPressed(KEY_ENTER)) {
        CloseWindow();  
    }
}

void LoadingDrawTextures(Loading *self) {

    if (self->state == LOGO_FADE_IN || self->state == LOGO_HOLD || self->state == LOGO_FADE_OUT) {
        DrawTexture(self->logo, SCREEN_WIDTH / 2 - self->logo.width / 2, SCREEN_HEIGHT / 2 - self->logo.height / 2, Fade(WHITE, self->alpha));
    } 
    else if (self->state == LOADING) {
        if (self->loadingVisible) {
            DrawText("Loading...", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, 20, BLACK);
        }
    } 
    else if (self->state == READY) {
        DrawText("Press Any Key to Start", SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2, 20, YELLOW);
    }
}

bool LoadingIsShow(Loading *self) {
    return self->ctx->app_state == APP_LOADING;
}

