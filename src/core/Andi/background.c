#include "background.h"

void Background_Draw(Background* self){
    // Rectangle rec = {40,40,40,40};
    // Vector2 origin = {0,0};
    // DrawRectanglePro(rec, origin, 50, RED);
}
void Background_Update(Background* self){

}
bool Background_IsShow(Background* self){
    if(self->ctx->app_state != APP_LOADING) {
        return true; 
    }
    return false;
}