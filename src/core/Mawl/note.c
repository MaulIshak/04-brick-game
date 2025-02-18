#include "raylib.h"
#include "gameplay.h"
#include "note.h"

void note_draw(DrawableNote *Note){

}
void note_update(DrawableNote *Note){
  
}
bool note_isShow(DrawableNote *Note){
  if(self->ctx->app_state == APP_PLAYING){
    return true;
  }
  return false;
}