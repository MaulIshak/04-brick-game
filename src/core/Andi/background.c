#include "background.h"
#include "raylib.h"
#include "kiss_fft.h"
#include "string.h"
// defined from https://github.com/tsoding/musializer/blob/762a729ff69ba1f984b0f2604e0eac08af46327c/src/plug.c#L69
// #define FFT_SIZE (1<<13)
#define FFT_SIZE (1024)

kiss_fft_cpx fft_in[FFT_SIZE];
kiss_fft_cpx fft_out[FFT_SIZE];

// adapted from https://github.com/tsoding/musializer/blob/762a729ff69ba1f984b0f2604e0eac08af46327c/src/plug.c#L410
void push_fft(float frame){
    memmove(fft_in, fft_in + 1, (FFT_SIZE - 1)*sizeof(kiss_fft_cpx));
    fft_in[FFT_SIZE-1].r = frame;
    fft_in[FFT_SIZE-1].i = 0;
}

void Background_MusicCallback(void *bufferData, unsigned int frames){
    float (*fs)[2] = bufferData;

    for (size_t i = 0; i < frames; i++) {
        push_fft(fs[i][0]);
    }
}

Background CreateBackground(AppContext* ctx){
    // TODO: use our own memory instead
    kiss_fft_cfg fft_cfg = kiss_fft_alloc(FFT_SIZE,0,NULL, NULL);
    // AttachAudioMixedProcessor(Background_MusicCallback);

    Background bg = {
        .ctx = ctx,
        .config = fft_cfg,
    };

    return bg;
}

void Background_Draw(Background* self){

}
void Background_Update(Background* self){
    // for(int i = 0; i < FFT_SIZE; i++) {
        // printf("%4f HZ | ", fft_out[i].r);
    // }
}

bool Background_IsShow(Background* self){
    if(self->ctx->app_state != APP_LOADING) {
        return true; 
    }
    return false;
}