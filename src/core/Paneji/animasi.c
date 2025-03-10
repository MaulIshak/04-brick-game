#include "context.h"
#include "animasi.h"

void LoadingLoadTextures(Loading *self) {
    self->texture[0] = LoadTexture("resources/texture/Arrow-3.png");  
    self->texture[1] = LoadTexture("resources/texture/Arrow-2.png");  
    self->texture[2] = LoadTexture("resources/texture/Arrow.png");    
    self->texture[3] = LoadTexture("resources/texture/Arrow-1.png");  
    self->texture[4] = LoadTexture("resources/texture/Arrow_YangAdaPutihnya-03.png");  
    self->texture[5] = LoadTexture("resources/texture/Arrow_YangAdaPutihnya-01.png");  
    self->texture[6] = LoadTexture("resources/texture/Arrow_YangAdaPutihnya-04.png");  
    self->texture[7] = LoadTexture("resources/texture/Arrow_YangAdaPutihnya-02.png");  
}

void LoadingUnloadTextures(Loading *self) {
    for (int i = 0; i < 8; i++) {
        UnloadTexture(self->texture[i]);
    }
}

void LoadingInitPositions(Loading *self) {
    int offsets[] = {0, 100, 200, 300, 150, 250, 50, 175};

    self->positions[0] = (Vector2){ SCREEN_WIDTH + offsets[0], SCREEN_HEIGHT / 3 };
    self->positions[1] = (Vector2){ -self->texture[1].width - offsets[1], 2 * SCREEN_HEIGHT / 3 };
    self->positions[2] = (Vector2){ SCREEN_WIDTH / 3, SCREEN_HEIGHT + offsets[2] };
    self->positions[3] = (Vector2){ 2 * SCREEN_WIDTH / 3, -self->texture[3].height - offsets[3] };
    self->positions[4] = (Vector2){ -self->texture[4].width - offsets[4], SCREEN_HEIGHT / 3 };
    self->positions[5] = (Vector2){ SCREEN_WIDTH + offsets[5], 2 * SCREEN_HEIGHT / 3 };
    self->positions[6] = (Vector2){ SCREEN_WIDTH / 3, -self->texture[6].height - offsets[6] };
    self->positions[7] = (Vector2){ 2 * SCREEN_WIDTH / 3, SCREEN_HEIGHT + offsets[7] };
}

void LoadingUpdatePositions(Loading *self) {
    self->positions[0].x -= TEXT_SPEED;
    self->positions[1].x += TEXT_SPEED;
    self->positions[2].y -= TEXT_SPEED;
    self->positions[3].y += TEXT_SPEED;

    self->positions[4].x += TEXT_SPEED;
    self->positions[5].x -= TEXT_SPEED;
    self->positions[6].y += TEXT_SPEED;
    self->positions[7].y -= TEXT_SPEED;

    if (self->positions[0].x < -self->texture[0].width) self->positions[0].x = SCREEN_WIDTH;
    if (self->positions[1].x > SCREEN_WIDTH) self->positions[1].x = -self->texture[1].width;
    if (self->positions[2].y < -self->texture[2].height) self->positions[2].y = SCREEN_HEIGHT;
    if (self->positions[3].y > SCREEN_HEIGHT) self->positions[3].y = -self->texture[3].height;

    if (self->positions[4].x > SCREEN_WIDTH) self->positions[4].x = -self->texture[4].width;
    if (self->positions[5].x < -self->texture[5].width) self->positions[5].x = SCREEN_WIDTH;
    if (self->positions[6].y > SCREEN_HEIGHT) self->positions[6].y = -self->texture[6].height;
    if (self->positions[7].y < -self->texture[7].height) self->positions[7].y = SCREEN_HEIGHT;
}

void LoadingDrawTextures(Loading *self) {
    for (int i = 0; i < 8; i++) {
        DrawTextureEx(self->texture[i], self->positions[i], 0.0f, 1.5f, WHITE);
    }
}

bool LoadingIsShow(Loading *self){
    return self->ctx->app_state == APP_LOADING;
}
