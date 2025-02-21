#include "raylib.h"
#include "gameplay.h"
#include "note.h"
#include "context.h"

void note_draw(DrawableNote *self){
  if(self->isBeatmapLoaded){
    // Gambar note disini
  }
}
void note_update(DrawableNote *self){
  if(!self->isBeatmapLoaded){
    self->beatmap = GetSelectedMusicBeatmap();
    self->isBeatmapLoaded = true;
  }
}
bool note_isShow(DrawableNote *self){
  if(self->ctx->app_state == APP_PLAYING){
    return true;
  }
  return false;
}

void InitNote(DrawableNote* self, AppContext *ctx){
  self->isBeatmapLoaded = false;
  self->ctx = ctx;
}
