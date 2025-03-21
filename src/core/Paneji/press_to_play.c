#include "press_to_play.h"
#include "context.h"
#include <stdio.h>

void PressToPlay_Draw(PressToPlay *self){
    // untuk menghilangkan warning, hapus ketika akan digunakan
  (void)self;
    DrawText("TEKAN APA SAJA UNTUK MULAI",SCREEN_WIDTH / 2 - 180 ,SCREEN_HEIGHT / 2 + 180, 23, BLACK);
}
void PressToPlay_Update(PressToPlay *self){
    if (IsKeyPressed(KEY_ENTER)) {
        self->ctx->app_state = APP_SELECT; // Ubah state ke APP_SELECT saat Enter ditekan
    }
}
bool PressToPlay_IsShow(PressToPlay *self){
    if(self->ctx->app_state == APP_PRESS_TO_PLAY) {
        return true;
    }
    return false;
}