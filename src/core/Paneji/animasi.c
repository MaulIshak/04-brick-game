#include "context.h"
#include "animasi.h"
#include "background.h"
#include "press_to_play.h"

void LoadingLoadTextures(Loading *self) {
    self->logo = LoadTexture("resources/texture/lambang-contoh.png"); 
    PressToPlay_LoadTextures(&self->ptp);
    self->intro = LoadSound("resources/sfx/coc-intro.wav");
    self->load = LoadSound("resources/sfx/sfx1.wav");
}

void LoadingUnloadTextures(Loading *self) {
    UnloadTexture(self->logo);
    UnloadSound(self->intro);
    UnloadSound(self->load);
}

void LoadingInitPositions(Loading *self) {
    self->alpha = 0.0f;
    self->timer = 0.0f;
    self->loadingVisible = true;
    self->state = LOGO_FADE_IN;
    
}


void LoadingUpdatePositions(Loading *self) {
    // static int count = 0;

    if (self->state == LOGO_FADE_IN) {
        DisableParticle();

        // if (!IsSoundPlaying(self->intro)) {
        //     PlaySound(self->intro);
        // }

        self->alpha += 0.02f;
        if (self->alpha >= 1.0f) {
            self->alpha = 1.0f;
            self->state = LOGO_HOLD;
            self->timer = 1.5f;
        }
    }else if (self->state == LOGO_HOLD) {
        self->timer -= GetFrameTime();
        if (self->timer <= 0) {
            self->state = LOGO_FADE_OUT;
        }
    }else if (self->state == LOGO_FADE_OUT) {
    self->alpha -= 0.02f;
    if (self->alpha <= 0.0f) {
        self->alpha = 0.0f;
        self->state = READY;  
    }
    }
    // else if (self->state == LOADING) {
    //     EnableParticle();
    //     self->timer += GetFrameTime();
    //     if (self->timer >= 0.5f) {
    //         self->loadingVisible = !self->loadingVisible;
    //         count++;
    //         PlaySound(self->load);
    //         self->timer = 0.0f;  
    //     }
    //     if (count == 7) {
    //         self->state = READY; 
    //     }
    else if (self->state == READY) { 
        PressToPlay_Update(&self->ptp); 
    }
}



void LoadingDrawTextures(Loading *self) {

    if (self->state == LOGO_FADE_IN || self->state == LOGO_HOLD || self->state == LOGO_FADE_OUT) {
        DrawTextureEx(self->logo, (Vector2){SCREEN_WIDTH / 2 - (self->logo.width / 2) / 2, SCREEN_HEIGHT / 2 - (self->logo.height / 2) / 2} , 0, 0.5, Fade(WHITE, self->alpha));
    } 
    // else if (self->state == LOADING) {
    //     if (self->loadingVisible) {
    //         DrawTextEx(self->ctx->font, "Loading", (Vector2){ SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 - 40 }, 40, 2, BLACK);

    //     }
     
    if (self->state == READY) { 
        PressToPlay_Draw(&self->ptp);
    }
}



bool LoadingIsShow(Loading *self) {
    return self->ctx->app_state == APP_LOADING;
}

