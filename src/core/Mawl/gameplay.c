#include "gameplay.h"
#include "raylib.h"
#include "context.h"

void gp_draw(Gameplay* self){
  

}
void gp_update(Gameplay* self){

}
bool gp_isShow(Gameplay* self){
  if(self->ctx->app_state == APP_PLAYING){
    return true;
  }
  return false;
}