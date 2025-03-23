#include "press_to_play.h"
#include "context.h"
#include <stdio.h>

void PressToPlay_LoadTextures(PressToPlay *self){
    self->logogame = LoadTexture("resources/texture/logo-game.png");
    self->posYlogo = -self->logogame.height * 0.4f;
    self->batu = LoadSound("resources/sfx/suara-batu.wav");
}

void PressToPlay_UnloadTextures(PressToPlay *self){
    UnloadTexture(self->logogame);
    UnloadSound(self->batu);
}



void PressToPlay_Update(PressToPlay *self){

    float targetY = SCREEN_HEIGHT * 0.3f - (self->logogame.height * 0.4f) / 3;

    if (self->posYlogo < targetY) {
        self->posYlogo += 1;

        if(!IsSoundPlaying(self->batu)){
            PlaySound(self->batu);
        }else{
            StopSound(self->batu);
        }
    }
    
    if (IsKeyPressed(KEY_F) || IsKeyPressed(KEY_J) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_K)) {
        self->ctx->app_state = APP_SELECT; 
    }
    
}

void PressToPlay_Draw(PressToPlay *self){
    
    DrawTextureEx(self->logogame, (Vector2){SCREEN_WIDTH / 2 - 350 , self->posYlogo}, 0.0f, 0.4f, WHITE);

    
    DrawTextEx(self->ctx->font, "TEKAN 'F' UNTUK MULAI", 
    (Vector2){ SCREEN_WIDTH / 2 - 157, SCREEN_HEIGHT / 2 + 180 }, 35, 2, BLACK);
    

}

bool PressToPlay_IsShow(PressToPlay *self){
    if(self->ctx->app_state == APP_PRESS_TO_PLAY) {
        return true;
    }
    return false;
}