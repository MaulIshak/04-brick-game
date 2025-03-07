#include "press_to_play.h"
#include "context.h"
#include <stdio.h>

// void PressToPlay_Draw(Loading *self){
//     // untuk menghilangkan warning, hapus ketika akan digunakan
//   (void)self;
//     DrawText("TEKAN APA SAJA UNTUK MULAI", 10,10, 16, BLACK);
// }
// void PressToPlay_Update(Loading *self){
//     if(IsKeyPressed(KEY_UP)) {
//         self->ctx->app_state = APP_SELECT;
//     }
// }
// bool PressToPlay_IsShow(Loading *self){
//     if(self->ctx->app_state == APP_PRESS_TO_PLAY) {
//         return true;
//     }
//     return false;
// }