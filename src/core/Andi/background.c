#include "background.h"
#include "raylib.h"
#include "kiss_fft.h"
#include "string.h"
// defined from https://github.com/tsoding/musializer/blob/762a729ff69ba1f984b0f2604e0eac08af46327c/src/plug.c#L69
#define FFT_SIZE (1<<13)
// #define FFT_SIZE (1024)

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

inline float amp(kiss_fft_cpx z)
{
    float a = z.r;
    float b = z.i;
    return logf(a*a + b*b);
}

Background CreateBackground(AppContext* ctx){
    // TODO: use our own memory instead
    kiss_fft_cfg fft_cfg = kiss_fft_alloc(FFT_SIZE,0,NULL, NULL);
    AttachAudioMixedProcessor(Background_MusicCallback);

    Background bg = {
        .ctx = ctx,
        .config = fft_cfg,
    };

    return bg;
}

void Background_Draw(Background* self){
    
    int bottom = self->ctx->screen_height;
    float step = 1.06;

    for(float freq = 1;  freq < FFT_SIZE; freq = (freq * step)) {
        int f1 = (int)ceilf(freq);
        int total = 0;
        int count = 0;
        int next_freq = (int)(freq * step);
        int avg = 0;

        for(int i = f1; i < next_freq && i < FFT_SIZE; i++) {
            total += fft_out[i].r;
            count++;
        }
        if (count > 0) {
            avg = total / count;
        }
        avg *= 3;
        DrawRectangle(freq, bottom - avg, 5, avg, Fade(RED, 0.125));
    }
}
void Background_Update(Background* self){

    for (int i = 0; i < FFT_SIZE; i++) {
        float t = (float)i/(FFT_SIZE - 1);
        float hann = 0.5 - 0.5*cosf(2*PI*t);
        fft_in[i].r = fft_in[i].r*hann;
    }

    kiss_fft(self->config, fft_in, fft_out);
    // for(int i = 0; i < FFT_SIZE; i++) {
        
    //     printf("%4f HZ \n", fft_out[i].r);
    // }
}

bool Background_IsShow(Background* self){
    if(self->ctx->app_state != APP_LOADING) {
        return true; 
    }
    return false;
}