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
  // Mulai timer/countdown untuk memulai game (3 detik)
  if(!self->timer.is_started) {
    timer_start(&self->timer, 3);
  }
  if(!self->musicTimer.is_started){
    timer_start(&self->musicTimer, 3 + ms_to_s(self->gp->gameTimeOffset));
  }
  if(!self->isTrackPlayed && is_timer_end(&(self->musicTimer))){
    PlaySelectedTrack(self->ctx);
    self->isTrackPlayed = true;
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
      _updateNotePosition(self);
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
    false,0,0
  };
  self->musicTimer = (Timer){
    false,0,0
  };
  self->gp = gp;
  self->isTrackPlayed = false;
  self->isFirstHit = false;
  self->acc = PERFECT;
  self->accOff = (AccuracyOffset){
    50, 50,
    100, 100,
    300, 300
  };

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
  if((IsKeyPressed(KEY_DOWN) || IsGamepadButtonPressed(0,GAMEPAD_BUTTON_LEFT_TRIGGER_1) ) && note.direction == NOTE_DOWN){
    if(self->gp->gameTime >= note.hit_at_ms-self->accOff.perfectUpperOffset && self->gp->gameTime <= note.hit_at_ms +self->accOff.perfectLowerOffset){
      self->acc = PERFECT; 
      printf("PERFECT\n");
      return true;
    }else if(self->gp->gameTime >= note.hit_at_ms-self->accOff.goodUpperOffset && self->gp->gameTime <= note.hit_at_ms + self->accOff.goodLowerOffset){
      printf("GOOD\n");
      self->acc = GOOD; 
      return true;
    } else if(self->gp->gameTime >= note.hit_at_ms-self->accOff.missUpperOffset && self->gp->gameTime <= note.hit_at_ms +self->accOff.missLowerOffset){
      printf("MISS\n");
      self->acc = MISS; 
      return true;
    }
    return false;
  }

  // LEFT ARROW (LEFT)
  if((IsKeyPressed(KEY_LEFT) || IsGamepadButtonPressed(0,GAMEPAD_BUTTON_LEFT_TRIGGER_2)) && note.direction == NOTE_LEFT){
    if(self->gp->gameTime >= note.hit_at_ms-self->accOff.perfectUpperOffset && self->gp->gameTime <= note.hit_at_ms +self->accOff.perfectLowerOffset){
      self->acc = PERFECT; 
      printf("PERFECT\n");
      return true;
    }else if(self->gp->gameTime >= note.hit_at_ms-self->accOff.goodUpperOffset && self->gp->gameTime <= note.hit_at_ms + self->accOff.goodLowerOffset){
      printf("GOOD\n");
      self->acc = GOOD; 
      return true;
    } else if(self->gp->gameTime >= note.hit_at_ms-self->accOff.missUpperOffset && self->gp->gameTime <= note.hit_at_ms +self->accOff.missLowerOffset){
      printf("MISS\n");
      self->acc = MISS; 
      return true;
    }
    return false;
  }


  // UP ARROW (MIDDLE RIGHT)
  if(IsKeyPressed(KEY_UP) || IsGamepadButtonPressed(0,GAMEPAD_BUTTON_RIGHT_TRIGGER_1) && note.direction == NOTE_UP){
    if(self->gp->gameTime >= note.hit_at_ms-self->accOff.perfectUpperOffset && self->gp->gameTime <= note.hit_at_ms +self->accOff.perfectLowerOffset){
      self->acc = PERFECT; 
      printf("PERFECT\n");
      return true;
    }else if(self->gp->gameTime >= note.hit_at_ms-self->accOff.goodUpperOffset && self->gp->gameTime <= note.hit_at_ms + self->accOff.goodLowerOffset){
      printf("GOOD\n");
      self->acc = GOOD; 
      return true;
    } else if(self->gp->gameTime >= note.hit_at_ms-self->accOff.missUpperOffset && self->gp->gameTime <= note.hit_at_ms +self->accOff.missLowerOffset){
      printf("MISS\n");
      self->acc = MISS; 
      return true;
    }
    return false;
  }

  // RIGHT ARROW (RIGHT)
  if(IsKeyPressed(KEY_RIGHT) || IsGamepadButtonPressed(0,GAMEPAD_BUTTON_RIGHT_TRIGGER_2) && note.direction == NOTE_RIGHT){
    if(self->gp->gameTime >= note.hit_at_ms-self->accOff.perfectUpperOffset && self->gp->gameTime <= note.hit_at_ms +self->accOff.perfectLowerOffset){
      self->acc = PERFECT; 
      printf("PERFECT\n");
      return true;
    }else if(self->gp->gameTime >= note.hit_at_ms-self->accOff.goodUpperOffset && self->gp->gameTime <= note.hit_at_ms + self->accOff.goodLowerOffset){
      printf("GOOD\n");
      self->acc = GOOD; 
      return true;
    } else if(self->gp->gameTime >= note.hit_at_ms-self->accOff.missUpperOffset && self->gp->gameTime <= note.hit_at_ms +self->accOff.missLowerOffset){
      printf("MISS\n");
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

void _updateNotePosition(DrawableNote* self){
  // Dapatkan frametime
  float dt = GetFrameTime();

  for (int i = 0; i <  self->beatmap.len; i++)
  {
    double elapsed = time_elapsed(&(self->timer));
    double to_hit = ms_to_s(self->beatmap.items[i].hit_at_ms);
    if(!(elapsed > to_hit - self->timeToHitPad)){
      continue;
    }
    if(!self->beatmap.items[i].isSpawned){
      self->beatmap.items[i].position.y = self->ctx->screen_height;
      self->beatmap.items[i].isSpawned = true;
    }
    float note_k = ( (self->ctx->screen_height-100)/self->timeToHitPad) * dt ; 
    self->beatmap.items[i].position.y -= note_k;

    _noteHitHandler(self, self->beatmap.items[i]);

    }
}

void _noteHitHandler(DrawableNote* self, Note note){
  // if(self->gp->gameTime >= note.hit_at_ms +self->accOff.missLowerOffset){
  //   // printf("Hit! : %d Time: %f\n", note.direction, self->gp->gameTime - 2000);
  //   self->acc = MISS;
  //   if(!self->isFirstHit){
  //     note.position.x= -999;
  //     self->isFirstHit = true;
  //   }
  // }

  if(_isNoteHit(self, note)){
    // printf("Hit!");
    note.position.x= -999;
    if(!self->isFirstHit){
      self->isFirstHit = true;
    }
  }else{
    
  }
}