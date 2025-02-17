#include "background.h"

void Background_Draw(Background* self){
    // DrawRectangle(20,20, 400, 400, SKYBLUE);
}
void Background_Update(Background* self){

}
bool Background_IsShow(Background* self){
    if(self->ctx->app_state != APP_LOADING) {
        return true; 
    }
    return false;
}