#include "raylib.h"
#include "gameplay.h"
#include "note.h"
#include "context.h"
#include "timer.h"
#include <stdio.h>

void note_draw(DrawableNote *self){
  if(self->isBeatmapLoaded){
      for(int i = 0; i < self->beatmap.len; i++) {
        // printf("x= %f y=%f\n", self->beatmap.items[0].position.x,self->beatmap.items[0].position.y );
        if(self->beatmap.items[i].position.y < 0) {
            continue;
        }
        _drawBeatmapNote(self, self->beatmap.items[i]);
      }
    }
}
void note_update(DrawableNote *self){
  float dt = GetFrameTime();
  if(!self->timer.is_started) {
    timer_start(&self->timer, 1);
}
  if(!self->isBeatmapLoaded){
    self->beatmap = GetSelectedMusicBeatmap(self->ctx);
    for (int i = 0; i < self->beatmap.len; i++)
    {
      self->beatmap.items[i].position.y = -99;
      self->beatmap.items[i].isSpawned = false;
    }
      self->isBeatmapLoaded = true;
      PlaySelectedTrack(self->ctx);
  }
  for (int i = 0; i <  self->beatmap.len; i++)
  {
    // Note *note = self->beatmap.items[i];
    double elapsed = time_elapsed(&(self->timer));
    double to_hit = ms_to_s(self->beatmap.items[i].hit_at_ms);
    // printf("elapsed: %d to hit: %d", elapsed, to_hit);
    if(!(elapsed > to_hit - self->timeToHitPad) ){
        continue;
    }
    if(!self->beatmap.items[i].isSpawned){
      self->beatmap.items[i].position.y = self->ctx->screen_height;
      self->beatmap.items[i].isSpawned = true;
    }
    float note_k = ( self->ctx->screen_height/self->timeToHitPad) * dt ; 
    self->beatmap.items[i].position.y -= note_k;
    // printf("y=%d\n", self->beatmap.items[i].position.y);
  }
  
}
bool note_isShow(DrawableNote *self){
  if(self->ctx->app_state == APP_PLAYING){
    return true;
  }
  return false;
}

void InitNote(DrawableNote *self, AppContext *ctx, Gameplay *gp){
  self->isBeatmapLoaded = false;
  self->ctx = ctx;
  self->timeToHitPad = 1;
  self->noteTexture[0] = LoadTexture("resources/texture/Arrow-3.png"); // panah kiri
  self->noteTexture[1] = LoadTexture("resources/texture/Arrow-1.png"); // panah bawah
  self->noteTexture[2] = LoadTexture("resources/texture/Arrow.png"); // panah atas;
  self->noteTexture[3] = LoadTexture("resources/texture/Arrow-2.png"); // panah kanan
  self->timer = (Timer){
    false,0,1
  };
  self->gp = gp;
}

void _drawBeatmapNote(DrawableNote* self, Note note){
  float loc =  self->ctx->screen_width/6;
  Texture2D textureToDraw;
  Vector2 position = note.position;
  switch (note.direction)
  {
  case NOTE_LEFT:
      position.x = self->gp->padPositions[0].x + 10 ;
      textureToDraw = self->noteTexture[0];
      break;
  case NOTE_RIGHT:
      position.x = self->gp->padPositions[3].x + 10;
      textureToDraw = self->noteTexture[3];
      break;
  case NOTE_UP:
      position.x = self->gp->padPositions[1].x + 10;
      textureToDraw = self->noteTexture[2];
      break;
  case NOTE_DOWN:
      position.x = self->gp->padPositions[2].x + 10;
      textureToDraw = self->noteTexture[1];
      break;
  }
  DrawTextureEx(textureToDraw, position,0,1.5,WHITE);
  
}