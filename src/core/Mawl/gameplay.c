#include "gameplay.h"
#include "raylib.h"
#include "context.h"
#include "note.h"
#include "macro.h"

void gp_draw(Gameplay* self){
  DrawTexture(self->textureToLoad, self->ctx->screen_height/2, self->ctx->screen_width/2, WHITE);
}
void gp_update(Gameplay* self){
  if(!self->isTextureLoaded){
    _loadTexture(self);
  }

}

bool gp_isShow(Gameplay* self){
  if(self->ctx->app_state == APP_PLAYING){
    return true;
  }
  return false;
}

void init_gameplay(Gameplay *gameplay, AppContext *ctx, int width){
  gameplay->ctx = ctx;
  gameplay->isTextureLoaded = false;
  gameplay->width = width;
  gameplay->texturePath = "arrow.png";
}

void _loadTexture(Gameplay*self){
  self->textureToLoad = LoadTexture(self->texturePath);
}