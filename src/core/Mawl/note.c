#include "raylib.h"
#include "gameplay.h"
#include "note.h"
#include "context.h"
#include "timer.h"
#include <stdio.h>

void note_draw(DrawableNote *self){
  // Cek apakah beatmap sudah diload
  if(self->isBeatmapLoaded){
      for(int i = 0; i < self->beatmap.len; i++) {
        if(self->beatmap.items[i].position.y < 0) {
            continue;
        }
        _drawBeatmapNote(self, self->beatmap.items[i]);

      }
    }

    // Mulai gambar akurasi hanya jika note pertama sampai
    if(self->isFirstHit){
      _drawAccuracy(self);
    }
}
void note_update(DrawableNote *self){

  // Dapatkan frametime
  float dt = GetFrameTime();

  // Mulai timer/countdown untuk memulai game (3 detik)
  if(!self->timer.is_started) {
    timer_start(&self->timer, 3);
  }

  // Inisialisasi posisi note jika beatmap sudah diload dan timer sudah selesai
  if(!self->isBeatmapLoaded && is_timer_end(&self->timer)){
    self->beatmap = GetSelectedMusicBeatmap(self->ctx);
    for (int i = 0; i < self->beatmap.len; i++)
    {
      self->beatmap.items[i].hit_at_ms += self->gp->gameTimeOffset;
      self->beatmap.items[i].position.y = -999;
      self->beatmap.items[i].isSpawned = false;
    }
      self->isBeatmapLoaded = true;
    }

    if(is_timer_end(&self->timer)){
      for (int i = 0; i <  self->beatmap.len; i++)
      {
        double elapsed = time_elapsed(&(self->timer));
        double to_hit = ms_to_s(self->beatmap.items[i].hit_at_ms);
        if(!(elapsed > to_hit - self->timeToHitPad)){
          continue;
        }
        if(!self->isTrackPlayed){
          PlaySelectedTrack(self->ctx);
          self->isTrackPlayed = true;
        }
        if(!self->beatmap.items[i].isSpawned){
          self->beatmap.items[i].position.y = self->ctx->screen_height;
          self->beatmap.items[i].isSpawned = true;
      }
      float note_k = ( (self->ctx->screen_height-100)/self->timeToHitPad) * dt ; 
      self->beatmap.items[i].position.y -= note_k;
      if(self->gp->gameTime >= self->beatmap.items[i].hit_at_ms-10 && self->gp->gameTime <= self->beatmap.items[i].hit_at_ms +10 && i >= 50){
        printf("Hit! : %d Time: %f index: %d\n", self->beatmap.items[i].direction, self->gp->gameTime - 2000, i);
      }

      if(_isNoteHit(self, self->beatmap.items[i])){
        printf("Hit!");
        self->beatmap.items[i].position.x= -999;
        if(!self->isFirstHit){
          self->isFirstHit = true;
        } 
      }
    }
    // printf("y= %f\n",self->beatmap.items[0].position.y);
    
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
  self->timeToHitPad = 1.5f;
  self->noteTexture[0] = LoadTexture("resources/texture/Arrow-3.png"); // panah kiri
  self->noteTexture[1] = LoadTexture("resources/texture/Arrow-1.png"); // panah bawah
  self->noteTexture[2] = LoadTexture("resources/texture/Arrow.png"); // panah atas;
  self->noteTexture[3] = LoadTexture("resources/texture/Arrow-2.png"); // panah kanan
  self->timer = (Timer){
    false,0,1
  };
  self->gp = gp;
  self->isTrackPlayed = false;
  self->isFirstHit = false;
  self->acc = PERFECT;

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

bool _isNoteHit(DrawableNote*self, Note note ){
            // DOWN ARROW (MIDDLE LEFT)
            if((IsKeyPressed(KEY_DOWN) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_TRIGGER_1) ) && note.direction == NOTE_DOWN){
              if(note.position.y < self->gp->padPositions[0].y + 30 && note.position.y > self->gp->padPositions[0].y - 10){ 
                self->acc = PERFECT; 
                return true;
              
              }
              if(note.position.y < self->gp->padPositions[0].y + 50 && note.position.y > self->gp->padPositions[0].y - 30){
                self->acc = GOOD; 
                return true;
              
              }
              if(note.position.y < self->gp->padPositions[0].y + 80 && note.position.y > self->gp->padPositions[0].y - 40){  
                self->acc = MISS; 
                return true;
              
              }
              return false;
            }
        
            // LEFT ARROW (LEFT)
            if((IsKeyPressed(KEY_LEFT) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_TRIGGER_2)) && note.direction == NOTE_LEFT){
              if(note.position.y < self->gp->padPositions[0].y + 30 && note.position.y > self->gp->padPositions[0].y - 10){ 
                self->acc = PERFECT; 
                return true;
              
              }
              if(note.position.y < self->gp->padPositions[0].y + 50 && note.position.y > self->gp->padPositions[0].y - 30){
                self->acc = GOOD; 
                return true;
              
              }
              if(note.position.y < self->gp->padPositions[0].y + 80 && note.position.y > self->gp->padPositions[0].y - 40){  
                self->acc = MISS; 
                return true;
              
              }
              return false;
            }
      
        
            // UP ARROW (MIDDLE RIGHT)
            if(IsKeyPressed(KEY_UP) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_RIGHT_TRIGGER_1) && note.direction == NOTE_UP){
              if(note.position.y < self->gp->padPositions[0].y + 20 && note.position.y > self->gp->padPositions[0].y - 5){ 
                self->acc = PERFECT; 
                return true;
              
              }
              if(note.position.y < self->gp->padPositions[0].y + 40 && note.position.y > self->gp->padPositions[0].y - 20){
                self->acc = GOOD; 
                return true;
              
              }
              if(note.position.y < self->gp->padPositions[0].y + 80 && note.position.y > self->gp->padPositions[0].y - 40){  
                self->acc = MISS; 
                return true;
              
              }
              return false;
            }
        
            // RIGHT ARROW (RIGHT)
            if(IsKeyPressed(KEY_RIGHT) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_RIGHT_TRIGGER_2) && note.direction == NOTE_RIGHT){
              if(note.position.y < self->gp->padPositions[0].y + 30 && note.position.y > self->gp->padPositions[0].y - 10){ 
                self->acc = PERFECT; 
                return true;
              
              }
              if(note.position.y < self->gp->padPositions[0].y + 50 && note.position.y > self->gp->padPositions[0].y - 30){
                self->acc = GOOD; 
                return true;
              
              }
              if(note.position.y < self->gp->padPositions[0].y + 80 && note.position.y > self->gp->padPositions[0].y - 40){  
                self->acc = MISS; 
                return true;
              
              }
              return false;
            }
            
        return false;

}

void _drawAccuracy(DrawableNote* self){
  char *accuracyText;
  Color color;
  switch (self->acc)
  {
  case PERFECT:
    accuracyText = "PERFECT";
    color = GREEN;
    break;
  case GOOD:
    accuracyText = "GOOD";
    color = YELLOW;
    break;
  case MISS:
    accuracyText = "MISS";
    color = RED;
    break;
  }
  DrawText(accuracyText, self->gp->width/2 - MeasureText(accuracyText,40)/2,self->gp->padPositions[0].y+100, 40, color);
}