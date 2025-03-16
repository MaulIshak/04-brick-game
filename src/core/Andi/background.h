#pragma once
#include "context.h"
#include "scene.h"
#include "kiss_fft.h"
#include "timer.h"
#include "stdint.h"

#ifndef BACKGROUND_H
#define BACKGROUND_H
#define PARTICLE_LEN 250
#define PATICLE_HEIGHT 200

typedef enum TransitionPhase {
    TRANSITION_NONE,
    TRANSITION_STARTING,
    TRANSITION_LOADING,
    TRANSITION_END,
} TransitionPhase;

typedef struct Particle {
    Vector2 pos;
    Vector2 speed;
    float frame_offset;
    float radius;
} Particle;

typedef struct ParticleConfig {
    Rectangle area; // Area kerja dari partikel
    

    Color start_color; // Warna dari partikel saat muncul pertama kali
    Color end_color;  // warna partikel akhir sebelum menghilang
} ParticleConfig;

typedef struct Background {
    AppContext* ctx;
    kiss_fft_cfg config;
    bool is_transition_running ;
    int start_transition_frame ;
    int transition_frame_delay;
    Texture2D transition_texture;
    TransitionPhase transition_phase;
    ParticleConfig particle_option;
    Particle particles[PARTICLE_LEN];
    Timer timer;
    uint64_t frame;
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

/**
 * Modul untuk mengaktifkan/menonaktifkan partikel.
 */

/* Konfigurasi partikel. Lihat struct `ParticleConfig` untuk selengkapnya.

Contoh penggunaan:
```c
SetParticleConfig((ParticleConfig){
        .area = {
            .height = self->ctx->screen_height,
            .width = self->ctx->screen_width,
        },
        .start_color = DARKGREEN,
        .end_color = RED
    });
```
*/
void SetParticleConfig(ParticleConfig cfg);
// Menyalakan partikel
void EnableParticle();
// Mematikan partikel
void DisableParticle();

impl_scene(Background*, Background_ToScene, Background_Draw, Background_Update, Background_IsShow)

#endif // BACKGROUND_H