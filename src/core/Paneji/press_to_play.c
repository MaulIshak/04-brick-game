#include "press_to_play.h"
#include "context.h"
#include <stdio.h>

void PressToPlay_LoadTextures(PressToPlay *self){
    self->logogame = LoadTexture("resources/texture/logo-game.png");
    self->posY = -self->logogame.height * 0.4f;
}

void PressToPlay_UnloadTextures(PressToPlay *self){
    UnloadTexture(self->logogame);
}

void PressToPlay_Draw(PressToPlay *self){
    
    DrawTextureEx(self->logogame, (Vector2){SCREEN_WIDTH / 2 - 350 , self->posY}, 0.0f, 0.4f, WHITE);

    
    DrawTextEx(self->ctx->font, "TEKAN 'F' UNTUK MULAI", 
    (Vector2){ SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 + 180 }, 35, 2, BLACK);
    

}

void PressToPlay_Update(PressToPlay *self){

    float targetY = SCREEN_HEIGHT / 2 - (self->logogame.height * 0.4f) / 2;

    if (self->posY < targetY) {
        self->posY += 5;
    }
    
    if (IsKeyPressed(KEY_F) || IsKeyPressed(KEY_J) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_K)) {
        self->ctx->app_state = APP_SELECT; 
    }
    
}
bool PressToPlay_IsShow(PressToPlay *self){
    if(self->ctx->app_state == APP_PRESS_TO_PLAY) {
        return true;
    }
    return false;
}