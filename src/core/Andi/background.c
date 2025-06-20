#include "background.h"
#include "raylib.h"
#include "kiss_fft.h"
#include "string.h"
#include "raymath.h"
#include "macro.h"
// defined from https://github.com/tsoding/musializer/blob/762a729ff69ba1f984b0f2604e0eac08af46327c/src/plug.c#L69
#define FFT_SIZE (1<<13)
// #define FFT_SIZE (1024)

kiss_fft_cpx fft_in[FFT_SIZE];
kiss_fft_cpx fft_out[FFT_SIZE];

static bool IsTransition = false;
static TransitionPhase _transition_phase = TRANSITION_NONE;
static ParticleConfig _particle_config = {0};
static bool _show_particle = false;

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

// compute amplitude
float amp(kiss_fft_cpx z)
{
    float a = z.r;
    float b = z.i;
    return logf(a*a + b*b);
}

Background CreateBackground(AppContext* ctx){
    // TODO: use our own memory instead
    // kiss_fft_cfg fft_cfg = kiss_fft_alloc(FFT_SIZE,0,NULL, NULL);
    // AttachAudioMixedProcessor(Background_MusicCallback);
    Texture2D transition_texture = LoadTexture("resources/texture/Transition.png");
    ParticleConfig opt = {
        .area = {.x = 0, .y = 0, .width = ctx->screen_width, .height = ctx->screen_height},
        .start_color = PRIMARY_COLOR,
        .end_color = SECONDARY_COLOR,
    };

    _particle_config = opt;

    Background bg = {   
        .ctx = ctx,
        // .config = fft_cfg,
        .is_transition_running = false,
        .start_transition_frame = 0,
        .transition_texture = transition_texture,
        .transition_phase = TRANSITION_NONE,
        .particles = {0},
        .timer = {0},
        .frame = 0,
        .particle_option = opt,  
    };

    bg.objects = FlyingObject_Create(ctx);

    return bg;
}

void Background_Draw(Background* self){

    DrawTextureEx(self->transition_texture, (Vector2){(-self->transition_texture.width) + (self->start_transition_frame * 40), 0},0, 1, WHITE);
    
    if(_show_particle) {
        for(int i = 0; i < PARTICLE_LEN; i++) {
            float factor_color = ((_particle_config.area.height + _particle_config.area.y) - self->particles[i].pos.y) / PATICLE_HEIGHT;
            float factor_alpha = 1.0 - ((_particle_config.area.height + _particle_config.area.y) - (PATICLE_HEIGHT / 2) - self->particles[i].pos.y) / (PATICLE_HEIGHT / 2) ;
            Color c = ColorLerp(_particle_config.start_color, _particle_config.end_color,  factor_color);
            c = ColorAlpha(c, factor_alpha);
            // buat posisi x seolah oleah (bergeser ke kiri dan ke kanan)
            float x_pos = self->particles[i].pos.x + (sinf( ((float)self->frame + self->particles[i].frame_offset) * self->particles[i].speed.x) * 3);
            DrawRectangleV((Vector2){x_pos, self->particles[i].pos.y}, (Vector2){self->particles[i].radius, self->particles[i].radius}, c);
        }
    }

}
void Background_Update(Background* self){
     
    if(IsTransition) {
        if(!self->is_transition_running) {
            self->is_transition_running = true;
            self->start_transition_frame = 0;
            self->transition_frame_delay = 0;
            self->transition_phase = TRANSITION_STARTING;
            IsTransition = false;
        }
    }
    
    if(self->is_transition_running) {
        
        if((self->start_transition_frame * 40) >= self->transition_texture.width / 3 * 2 && self->transition_phase == TRANSITION_STARTING) {
            self->transition_phase = TRANSITION_LOADING;
        }

        if(self->transition_phase == TRANSITION_LOADING) {
            self->transition_frame_delay++;
            if(self->transition_frame_delay >= 60) {
                self->transition_phase = TRANSITION_END;
            }
        }

        if(self->transition_phase != TRANSITION_LOADING) {
            self->start_transition_frame++;
        }

        if(self->start_transition_frame > 80) {
            self->transition_phase = TRANSITION_NONE;
            self->is_transition_running = false;
        }

    }

    // Particle
    for(int i = 0; i < PARTICLE_LEN; i++) {
        int w = _particle_config.area.width;
        if(self->particles[i].pos.y == 0) {
            self->particles[i].pos.y = _particle_config.area.y + _particle_config.area.height - GetRandomValue(0, PATICLE_HEIGHT); 
            self->particles[i].speed.y = GetRandomValue(3, 5) * 0.1;
            self->particles[i].radius = GetRandomValue(2,5);
        }else {
            // Particle despawn
            if(self->particles[i].pos.y < _particle_config.area.y + _particle_config.area.height - PATICLE_HEIGHT) {
                self->particles[i].pos.y = _particle_config.area.y + _particle_config.area.height;
                self->particles[i].speed.y = GetRandomValue(3, 5) * 0.1;
                self->particles[i].speed.x = GetRandomValue(1,10) * 0.01;
                self->particles[i].speed.x = 0.02;
                self->particles[i].frame_offset = GetRandomValue(-10, 10);
                self->particles[i].radius = GetRandomValue(2,5);
            };
            self->particles[i].pos.y -= self->particles[i].speed.y;
        }

        if(self->particles[i].pos.x == 0) {
            self->particles[i].pos.x = (w / PARTICLE_LEN * i) + 3 + _particle_config.area.x;    
            self->particles[i].speed.x = GetRandomValue(1,10) * 0.01;
            self->particles[i].frame_offset = GetRandomValue(-10, 10);
        }
    }

    _transition_phase = self->transition_phase;   
    self->frame++;
}

bool Background_IsShow(Background* self){
    UNUSED(self);
    return true;
}

void StartTransition() {
    IsTransition = true;
}

bool ShouldChangeScene() {
    return _transition_phase == TRANSITION_LOADING;
}

bool IsTransitionEnd() {
    return _transition_phase == TRANSITION_NONE;
}

void SetParticleConfig(ParticleConfig cfg) {
    _particle_config = cfg;
}

void EnableParticle() {
    _show_particle = true;
}

void DisableParticle() {
    _show_particle = false;
}