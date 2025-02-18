#include "raylib.h"
#include "context.h"
#include "note.h"
#include "macro.h"
#include "gameplay.h"

void gp_draw(Gameplay* self){
  // for (int i = 0; i < TEXTURE_COUNT; i++)
  // {
  //   DrawTextureEx(self->textureToLoad[0], (Vector2){10, 10},.0f, .5f, WHITE);
  // }
}
void gp_update(Gameplay* self){

}

bool gp_isShow(Gameplay* self){
  if(self->ctx->app_state == APP_PLAYING){
    return true;
  }
  return false;
}

void InitGameplay(Gameplay *gameplay, AppContext *ctx){
  gameplay->ctx = ctx;
  gameplay->width = 400;
  gameplay->texturePath[0] = "src/core/Mawl/arrow.png";
  _LoadNoteTexture(gameplay);
}

void _LoadNoteTexture(Gameplay*self){
  for (int i = 0; i < TEXTURE_COUNT; i++)
  {
    self->textureToLoad[i] = LoadTexture(self->texturePath[i]);
  }
  
}