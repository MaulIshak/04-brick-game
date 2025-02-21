#include "background.h"
#include "raylib.h"

void Background_MusicCallback(void *bufferData, unsigned int frames){
    (void)bufferData;
    (void)frames;
}

Background CreateBackground(AppContext* ctx){
    AttachAudioMixedProcessor(Background_MusicCallback);

    return (Background){
        .ctx = ctx
    };
}

void Background_Draw(Background* self){

}
void Background_Update(Background* self){

}

bool Background_IsShow(Background* self){
    if(self->ctx->app_state != APP_LOADING) {
        return true; 
    }
    return false;
}