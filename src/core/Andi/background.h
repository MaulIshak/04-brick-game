#pragma once
#include "context.h"
#include "scene.h"
#include "kiss_fft.h"

#ifndef BACKGROUND_H
#define BACKGROUND_H

typedef enum TransitionPhase {
    TRANSITION_NONE,
    TRANSITION_STARTING,
    TRANSITION_LOADING,
    TRANSITION_END,
} TransitionPhase;

typedef struct Background {
    AppContext* ctx;
    kiss_fft_cfg config;
    bool is_transition_running ;
    int start_transition_frame ;
    int transition_frame_delay;
    Texture2D transition_texture;
    TransitionPhase transition_phase;
} Background;

void push_fft(float frame);

void Background_MusicCallback(void *bufferData, unsigned int frames);
Background CreateBackground(AppContext* ctx);
void Background_Draw(Background* self);
void Background_Update(Background* self);
bool Background_IsShow(Background* self);

/**
Modul untuk melakukan transisi. Biasanya digunakan untuk mengubah scene.

Untuk pengubah, biasanya algoritma nya sebagai berikut.
```c
if(SesuatuYangTujuannyaMengubahScene) {
    StartTransition();
    if (ShouldChangeScene()) {
        self->ctx->app_state = (SCENE LAIN)
    }
}
```

Sedang untuk penerima perubahan, algoritma nya sebagai berikut.

if(IsTransitionEnd()) {
  // Lakukan inisialisasi update disini
}
 */
void StartTransition();
bool ShouldChangeScene();
bool IsTransitionEnd();

impl_scene(Background*, Background_ToScene, Background_Draw, Background_Update, Background_IsShow)

#endif // BACKGROUND_H