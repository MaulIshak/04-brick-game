#include "press_to_play.h"
#include "context.h"
#include <stdio.h>

void PressToPlay_Draw(PressToPlay *self){
    Vector2 textVector = MeasureTextEx(self->ctx->font, "TEKAN APA SAJA UNTUK MULAI", 35, 2);
    DrawTextEx(self->ctx->font, "TEKAN APA SAJA UNTUK MULAI", (Vector2){self->ctx->screen_width/2 - textVector.x/2, self->ctx->screen_height/2 - textVector.y/2}, 35, 2, BLACK);
}
void PressToPlay_Update(PressToPlay *self){
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) {
        self->ctx->app_state = APP_SELECT; 
    }
    
}
bool PressToPlay_IsShow(PressToPlay *self){
    if(self->ctx->app_state == APP_PRESS_TO_PLAY) {
        return true;
    }
    return false;
}