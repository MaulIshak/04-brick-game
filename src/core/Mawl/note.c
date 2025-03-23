#include "raylib.h"
#include "gameplay.h"
#include "note.h"
#include "context.h"
#include "timer.h"
#include <stdio.h>
#include "sfx.h"

// static double alpha = 255;
double noteoffset;
int prev_track = -1;

void note_draw(NoteManager *self){
  // Rectangle rec = {
  //   0,0,self->gp->width, self->ctx->screen_height
  // };
  // DrawRectangleRec(rec, Fade(WHITE, sealpha/255));
  // Mulai gambar akurasi hanya jika note pertama sampai
  if(self->isFirstHit){
    _drawAccuracy(self);
  }
  // Cek apakah beatmap sudah diload
  if(self->isBeatmapLoaded){
      for(int i = 0; i < self->beatmap.len; i++) {
        if(self->note[i].position.y < 0) {
            continue;
        }
          // printf("Pos Y = %.2f isHit: %d\n",self->note[i].position.y, self->note[i].isHit);
          _drawBeatmapNote(self, self->note[i]);
          // printf("Kegambar!\n");
      }
    }

}
void note_update(NoteManager *self){
  // Mulai timer/countdown untuk memulai game (3 detik)
  if(!self->timer.is_started) {
    // self->gp->gameTime = 0;
    timer_start(&self->timer, 3);
  }
  if(!self->musicTimer.is_started){
    // self->gp->gameTime = 0;
    timer_start(&self->musicTimer, 3 + ms_to_s(noteoffset));
  }
  if(!self->isTrackPlayed && is_timer_end(&(self->musicTimer))){
    // for (int i = 0; i < self->beatmap.len; i++) {
    //   self->note[i].isHit = 0;
    // }
    printf("Music start time: %f\n", self->gp->gameTime);
    PlaySelectedTrack(self->ctx);
    
    self->isTrackPlayed = true;
  }
  if(IsKeyPressed(KEY_SPACE)){
    SeekSelectedTrack(self->ctx, 29);
  }
  if(self->isTrackPlayed){
     if(IsSelectedMusicEnd(self->ctx) ){
        self->ctx->app_state = END_OF_THE_GAME;
        prev_track = self->ctx->selected_track;
        self->isTrackPlayed = false;
        self->isBeatmapLoaded = false;
        self->isFirstHit = false;
        self->timer.is_started = false;
        self->musicTimer.is_started = false;
        self->isNewGame = false;
        self->gp->timer.is_started = false;
        self->gp->gameTime = 0;
        // self->gp->gameTimeOffset = 2000;
        _resetNoteManager(self); // Ensure all note-related states are reset
        return;
      }
    }  
    // Inisialisasi posisi note jika beatmap sudah diload dan timer sudah selesai
    if(!self->isBeatmapLoaded && is_timer_end(&self->timer)){
      if(prev_track == self->ctx->selected_track){
        self->gp->gameTimeOffset = 0;
      }else{
        self->gp->gameTimeOffset = noteoffset;
      }
      printf("%.2f\n\n", self->gp->gameTime);
      // printf("SELECTED\n");
      self->beatmap = GetSelectedMusicBeatmap(self->ctx);
      for (int i = 0; i < self->beatmap.len; i++)
      {
        // self->beatmap.items[i].hit_at_ms += self->gp->gameTimeOffset;
        self->beatmap.items[i].position.y = -999;
        self->beatmap.items[i].isSpawned = false;
        self->note[i].isHit = false;
        _extractNoteFromBeatmap(self);
      }
        self->isBeatmapLoaded = true;
    }
    

    if(is_timer_end(&self->timer)){
      _updateNotePosition(self);
    }

  
}

bool note_isShow(NoteManager *self){
  if(self->ctx->app_state == APP_PLAYING){
    return true;
  }
  return false;
}

void InitNote(NoteManager *self, AppContext *ctx, Gameplay *gp, ScoreManager *scoreManager){
  self->isBeatmapLoaded = false;
  self->ctx = ctx;
  self->timeToHitPad = 1.5f;
  self->noteTexture[0] = LoadTexture("resources/texture/Arrow-01.png"); // panah kiri
  self->noteTexture[1] = LoadTexture("resources/texture/Arrow-03.png"); // panah bawah
  self->noteTexture[2] = LoadTexture("resources/texture/Arrow-02.png"); // panah atas;
  self->noteTexture[3] = LoadTexture("resources/texture/Arrow-04.png"); // panah kanan
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
    70, 70,
    80, 80
  };
  self->scoreManager = scoreManager;
  self->isBeatmapLoaded = false;
  self->isNewGame = true;
  noteoffset = self->gp->gameTimeOffset;
}

void _drawBeatmapNote(NoteManager* self, DrawableNote note){
  Texture2D textureToDraw;
  Vector2 position = note.position;
  switch (note.direction)
  {
  case NOTE_LEFT:
      position.x = self->gp->padPositions[0].x;
      textureToDraw = self->noteTexture[0];
      break;
  case NOTE_RIGHT:
      position.x = self->gp->padPositions[3].x;
      textureToDraw = self->noteTexture[3];
      break;
  case NOTE_UP:
      position.x = self->gp->padPositions[1].x;
      textureToDraw = self->noteTexture[2];
      break;
  case NOTE_DOWN:
      position.x = self->gp->padPositions[2].x;
      textureToDraw = self->noteTexture[1];
      break;
  }

  if(!note.isHit){
    // DrawLineEx((Vector2){self->gp->padPositions[0].x,position.y + self->gp->padSize/2}, (Vector2){self->gp->padPositions[3].x + self->gp->padSize, position.y + self->gp->padSize/2}, 5, BLACK);
    DrawTextureEx(textureToDraw, position,0,.16,WHITE);
  }

  
}

bool _isNoteHit(NoteManager*self, DrawableNote note ){
  double ps = self->gp->padSize;
  double notePos = note.position.y + ps/2;
  double padY = self->gp->padPositions[0].y;
  // Akurasi berdasarkan waktu
  bool isPerfect = self->gp->gameTime >= note.hit_at_ms-self->accOff.perfectUpperOffset && self->gp->gameTime <= note.hit_at_ms +self->accOff.perfectLowerOffset;
  bool isGood =self->gp->gameTime >= note.hit_at_ms-self->accOff.goodUpperOffset && self->gp->gameTime <= note.hit_at_ms + self->accOff.goodLowerOffset;
  bool isMiss =self->gp->gameTime >= note.hit_at_ms-self->accOff.missUpperOffset && self->gp->gameTime <= note.hit_at_ms +self->accOff.missLowerOffset;

  (void) isPerfect;
  (void) isGood;
  (void) isMiss;
  // Akurasi berdasarkan posisi
  bool isPerfectPos = notePos >= padY + 20 && notePos <= padY + self->gp->padSize - 20;
  bool isGoodPos = notePos >= padY  && notePos <= padY + self->gp->padSize;
  bool isMissPos = notePos >= padY - 20 && notePos <= padY + self->gp->padSize + 20;
  // DOWN ARROW (MIDDLE RIGHT)
  if((IsKeyPressed(KEY_J) ||IsKeyPressed(KEY_J)|| IsGamepadButtonPressed(0,GAMEPAD_BUTTON_RIGHT_TRIGGER_1) ) && note.direction == NOTE_DOWN){
    
    if(isPerfectPos){
      // PlayPerfectSfx();
      self->acc = PERFECT; 
      return true;
    }else if(isGoodPos){
      // PlayGoodSfx();
      printf("GOOD\n");
      self->acc = GOOD; 
      return true;
    } else if(isMissPos){
      // PlayMissSfx();
      printf("MISS\n");
      self->acc = MISS; 
      return true;
    }
    return false;
  }

  // LEFT ARROW (LEFT)
  if((IsKeyPressed(KEY_D) ||IsKeyPressed(KEY_D)|| IsGamepadButtonPressed(0,GAMEPAD_BUTTON_LEFT_TRIGGER_2)) && note.direction == NOTE_LEFT){
    if(isPerfectPos){
      // PlayPerfectSfx();
      self->acc = PERFECT; 
      printf("PERFECT\n");
      return true;
    }else if(isGoodPos){
      // PlayGoodSfx();
      printf("GOOD\n");
      self->acc = GOOD; 
      return true;
    } else if(isMissPos){
      // PlayMissSfx();
      printf("MISS\n");
      self->acc = MISS; 
      return true;
    }
    return false;
  }


  // UP ARROW (MIDDLE LEFT)
  if((IsKeyPressed(KEY_F) ||IsKeyPressed(KEY_F)|| IsGamepadButtonPressed(0,GAMEPAD_BUTTON_LEFT_TRIGGER_1)) && note.direction == NOTE_UP){
    if(isPerfectPos){
      // PlayPerfectSfx();
      self->acc = PERFECT; 
      printf("PERFECT\n");
      // printf("abcsdsdsd\n\n\n");
      return true;
    }else if(isGoodPos){
      // PlayGoodSfx();
      printf("GOOD\n");
      self->acc = GOOD; 
      return true;
    } else if(isMissPos){
      // PlayMissSfx();
      printf("MISS\n");
      self->acc = MISS; 
      return true;
    }
    return false;
  }

  // RIGHT ARROW (RIGHT)
  if((IsKeyPressed(KEY_K) ||IsKeyPressed(KEY_K)|| IsGamepadButtonPressed(0,GAMEPAD_BUTTON_RIGHT_TRIGGER_2)) && note.direction == NOTE_RIGHT){
    if(isPerfectPos){
      // PlayPerfectSfx();
      self->acc = PERFECT; 
      printf("PERFECT\n");
      return true;
    }else if(isGoodPos){
      // PlayGoodSfx();
      printf("GOOD\n");
      self->acc = GOOD; 
      return true;
    } else if(isMissPos){
      // PlayMissSfx();
      printf("MISS\n");
      self->acc = MISS; 
      return true;
    }
    return false;
  }
  
  return false;

}

void _drawAccuracy(NoteManager* self){
  char *accuracyText;
  Color color;
  switch (self->acc)
  {
  case PERFECT:
    accuracyText = "PERFECT!!";
    color = GREEN;
    break;
  case GOOD:
    accuracyText = "GOOD!";
    color = YELLOW;
    break;
  case MISS:
    accuracyText = "MISS?";
    color = RED;
    break;
  }


  Vector2 pos =(Vector2) {
    self->gp->width/2 - MeasureTextEx(self->ctx->font, accuracyText,50,1).x/2,self->gp->padPositions[0].y+150
  };

  self->gp->alpha -= GetFrameTime() * 220;
  // DrawTextEx(self->ctx->font, accuracyText, pos, 100, 1, Fade(WHITE, self->gp->alpha/255));
  DrawTextEx(self->ctx->font, accuracyText, pos, 50, 1, Fade(color, self->gp->alpha/255));
}

void _updateNotePosition(NoteManager* self){
  // Dapatkan frametime
  float dt = GetFrameTime();
  for (int i = 0; i <  self->beatmap.len; i++)
  {
    double elapsed = time_elapsed(&(self->timer));
    double to_hit = ms_to_s(self->note[i].hit_at_ms);
    if(!(elapsed > to_hit - self->timeToHitPad)){
      // printf("elapsed: %f!\t", elapsed);
      // printf(": %f!\n", to_hit - self->timeToHitPad);
      continue;
    }
    if(!self->note[i].isSpawned){
      self->note[i].position.y = self->ctx->screen_height;
      self->note[i].isSpawned = true;
    }
    float note_k = ( (self->ctx->screen_height - 45)/self->timeToHitPad) * dt ; 
    self->note[i].position.y -= note_k;
    if(self->note[i].position.y>0){

    }

    _noteHitHandler(self, &(self->note[i]));

    }
}

void _noteHitHandler(NoteManager* self, DrawableNote *note){
  // Time miss
  bool isMiss = self->gp->gameTime >= note->hit_at_ms +self->accOff.missLowerOffset;
  // Position miss
  bool isMissPos = note->position.y + self->gp->padSize/2 < self->gp->padPositions[0].x - 50;
  if(isMissPos){
    if(!note->isHit){
      // printf("Time: %f\n", self->gp->gameTime);
      // printf("Note Hit at Ms : %f\n", note->hit_at_ms);
      self->gp->alpha = 255;
      note->isHit = true; // BUGNYA DISINI!
      // printf("MISS\n");
      self->isFirstHit = true;
      self->acc = MISS;
      AddScore(self->scoreManager, self->acc);
    }
  }
  //   // printf("Hit! : %d Time: %f\n", note.direction, self->gp->gameTime - 2000);
  //   self->acc = MISS;
  //   if(!self->isFirstHit){
  //     note.position.x= -999;
  //     self->isFirstHit = true;
  //   }
  // }
  if(note->isHit){
    note->position.x= -999;
  }

  if(!note->isHit){
    if(_isNoteHit(self, *note)){
      self->gp->alpha = 255;
      note->isHit = true;
      AddScore(self->scoreManager, self->acc);
      // printf("Hit!");
      if(!self->isFirstHit){
        self->isFirstHit = true;
      }
    }
  }
}

void _extractNoteFromBeatmap(NoteManager* self){
  for (int i = 0; i < self->beatmap.len && i < 1024; i++)
  {
    self->note[i].direction = self->beatmap.items[i].direction; 
    self->note[i].hit_at_ms = self->beatmap.items[i].hit_at_ms + self->gp->gameTimeOffset; 
    self->note[i].position = self->beatmap.items[i].position; 
    self->note[i].isHit = 0;
    self->note[i].isSpawned = false;
  }
  
}

void _resetNoteManager(NoteManager *self) {
  self->isBeatmapLoaded = false;
  self->isTrackPlayed = false;
  self->isFirstHit = false;
  self->acc = PERFECT;
  self->timer.is_started = false;
  self->musicTimer.is_started = false;
  self->gp->gameTime = 0;
  for (int i = 0; i < self->beatmap.len; i++) {
    self->note[i].position.y = -999;
    self->note[i].isHit = 0;
    self->note[i].isSpawned = false;
  }
}

