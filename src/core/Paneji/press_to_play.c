#include "press_to_play.h"
#include "context.h"
#include <stdio.h>

void PressToPlay_Draw(PressToPlay *self){
  DrawTextEx(self->ctx->font, "TEKAN APA ARROW UP UNTUK MULAI", 
    (Vector2){ SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 + 180 }, 
    35, 2, BLACK);

}
void PressToPlay_Update(PressToPlay *self){
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