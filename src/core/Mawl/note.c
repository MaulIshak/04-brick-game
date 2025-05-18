#include "raylib.h"
#include "gameplay.h"
#include "note.h"
#include "context.h"
#include "timer.h"
#include <stdio.h>
#include <math.h>

char text[1024];

void note_draw(NoteManager *self){
  // Mulai gambar akurasi hanya jika note pertama sampai
  if(self->isFirstHit){
    _drawAccuracy(self);
  }
  // Cek apakah beatmap sudah diload
  if(self->isBeatmapLoaded){
      for(int i = 0; i < self->beatmap.len; i++) {
        _drawNoteTrail(self, self->note[i]);
        if(self->note[i].position.y < 0) {
            continue;
        }
        _drawBeatmapNote(self, self->note[i]);
          // printf("Pos Y = %.2f isHit: %d\n",self->note[i].position.y, self->note[i].isHit);
          // printf("Kegambar!\n");
      }
    }
    if(self->gp->life <= 0 && self->ctx->app_state == APP_PLAYING){
      self->ctx->app_state = END_OF_THE_GAME;
      StopSelectedTrack(self->ctx);
      _resetNoteManager(self);
      // DrawText("FAIL", GetScreenWidth()/2 - MeasureText("FAIL", 100)/2, GetScreenHeight()/2, 100, BLACK);
      self->gp->life = 100;
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
    timer_start(&self->musicTimer, 3 + ms_to_s(self->gp->gameTimeOffset));
  }
  if(!self->isTrackPlayed && is_timer_end(&(self->musicTimer))){
    // for (int i = 0; i < self->beatmap.len; i++) {
      //   self->note[i].isHit = 0;
      // }
      printf("Music start time: %f\n", self->gp->gameTime);
      PlaySelectedTrack(self->ctx);
      self->isTrackPlayed = true;
      self->gp->isPlaying = true;
    }
  if(IsKeyPressed(KEY_ENTER)){
      self->gp->isPlaying = !self->gp->isPlaying;
  }
  if(!self->gp->isPlaying){
    return;
  }
  if(self->isTrackPlayed){
     if(IsSelectedMusicEnd(self->ctx) ){
        self->ctx->app_state = END_OF_THE_GAME;
        self->isTrackPlayed = false;
        self->isBeatmapLoaded = false;
        self->isFirstHit = false;
        self->timer.is_started = false;
        self->musicTimer.is_started = false;
        self->gp->timer.is_started = false;
        self->gp->gameTime = 0;
        self->gp->isBackgroundLoaded = false;
        _resetNoteManager(self); // Ensure all note-related states are reset
        return;
      }
    }  
    // Inisialisasi posisi note jika beatmap sudah diload dan timer sudah selesai
    if(!self->isBeatmapLoaded && is_timer_end(&self->timer)){
      printf("%.2f\n\n", self->gp->gameTime);
      self->beatmap = GetSelectedMusicBeatmap(self->ctx);
      for (int i = 0; i < self->beatmap.len; i++)
      {
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
  self->missCombo = 0;
  // noteoffset = self->gp->gameTimeOffset;
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

  Shader shader;
  
}

bool _isNoteHit(NoteManager*self, DrawableNote note ){
  double ps = self->gp->padSize;
  double notePos = note.position.y + ps/2;
  double padY = self->gp->padPositions[0].y;
  // Akurasi berdasarkan waktu (Untuk saat ini tidak digunakan)
  // bool isPerfect = self->gp->gameTime >= note.hit_at_ms-self->accOff.perfectUpperOffset && self->gp->gameTime <= note.hit_at_ms +self->accOff.perfectLowerOffset;
  // bool isGood =self->gp->gameTime >= note.hit_at_ms-self->accOff.goodUpperOffset && self->gp->gameTime <= note.hit_at_ms + self->accOff.goodLowerOffset;
  // bool isMiss =self->gp->gameTime >= note.hit_at_ms-self->accOff.missUpperOffset && self->gp->gameTime <= note.hit_at_ms +self->accOff.missLowerOffset;
  // (void) isPerfect;
  // (void) isGood;
  // (void) isMiss;
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
      self->acc = PERFECT; 
      printf("PERFECT\n");
      return true;
    }else if(isGoodPos){
      printf("GOOD\n");
      self->acc = GOOD; 
      return true;
    } else if(isMissPos){
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
    self->gp->width/2 - MeasureTextEx(self->ctx->font, accuracyText,50,1).x/2 - 40,self->gp->padPositions[0].y+150
  };

  self->gp->alpha -= GetFrameTime() * 220;
  DrawTextEx(self->ctx->font, accuracyText, pos, 50, 1, Fade(color, self->gp->alpha/255));
}

void _updateNotePosition(NoteManager* self){
  // Dapatkan frametime
  float dt = GetFrameTime();
  for (int i = 0; i <  self->beatmap.len; i++)
  {
    double elapsed = time_elapsed(&(self->timer));
    double to_hit = ms_to_s(self->note[i].hit_at_ms);

    // Jika belum waktunya muncul, maka lanjutkan iterasi (skip)
    if(!(elapsed > to_hit - self->timeToHitPad)){
      continue;
    }

    // Jika sudah waktunya untuk muncul dan not belum muncul, maka munculkan note;
    if(!self->note[i].isSpawned){
      self->note[i].position.y = self->ctx->screen_height;
      self->note[i].isSpawned = true;
    }
    float note_k = ( (self->ctx->screen_height - 45)/self->timeToHitPad) * dt ; 
    self->note[i].position.y -= note_k;
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
      self->gp->alpha = 255;
      note->isHit = true;
      self->isFirstHit = true;
      self->acc = MISS;
      AddScore(self->scoreManager, self->acc);
      UpdateLife(self->gp, self->acc);
    }
  }
  if(note->isHit){
    note->position.x= -999;
  }

  if(!note->isHit){
    if(_isNoteHit(self, *note)){
      self->gp->alpha = 255;
      note->isHit = true;
      AddScore(self->scoreManager, self->acc);
      UpdateLife(self->gp, self->acc);
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
    self->note[i].duration_in_ms = self->beatmap.items[i].duration_in_ms;
  }
  
}

void _resetNoteManager(NoteManager *self) {
  self->gp->timer.is_started = false;
  self->gp->gameTime = 0;
  self->gp->isBackgroundLoaded = false;
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

void _drawNoteTrail(NoteManager* self, DrawableNote note){
  Vector2 position = note.position;
  Color trailColor1;
  Color trailColor2;
  switch (note.direction)
  {
  case NOTE_LEFT:
      position.x = self->gp->padPositions[0].x;
      trailColor1 = (Color){
        0xA6, 0x38, 0xEF, 0xFF
      };
      trailColor2 = (Color){
        0x22, 0x72, 0xC7, 0xFF
      };
      break;
  case NOTE_RIGHT:
      position.x = self->gp->padPositions[3].x;
      trailColor1 = (Color){
        0xFD, 0xF1, 0x71, 0xFF
      };
      trailColor2 = (Color){
        0xF9, 0x84, 0xB9, 0xFF
      };
      break;
  case NOTE_UP:
      position.x = self->gp->padPositions[1].x;
      trailColor1 = (Color){
        0xF8, 0x53, 0x51, 0xFF
      };
      trailColor2 = (Color){
        0x63, 0x21, 0x79, 0xFF
      };
      break;
  case NOTE_DOWN:
      position.x = self->gp->padPositions[2].x;
      trailColor1 = (Color){
        0x4B, 0xF7, 0xFE, 0xFF
      };
      trailColor2 = (Color){
        0xAE, 0xAD, 0x3B, 0xFF
      };
      break;
  }

  if (note.duration_in_ms > 0) {
    float holdLength = (note.duration_in_ms / 1000.0f) * ( (self->ctx->screen_height - 45) / self->timeToHitPad );

    Rectangle holdBody = {
        position.x + self->gp->padSize/2 -self->gp->padSize/4 ,
        position.y + self->gp->padSize/2, // sedikit di bawah head
        self->gp->padSize/2, // lebar body
        holdLength // panjang body ke bawah
    };

    // DrawRectangleRec(holdBody, BLACK); // Warna abu2 transparan utk body
    BeginScissorMode(holdBody.x, self->gp->padPositions[0].y + self->gp->padSize/2, holdBody.width, GetScreenHeight());
    DrawRectangleGradientV(holdBody.x, holdBody.y, holdBody.width, holdBody.height, trailColor1, trailColor2);
    EndScissorMode();
  }
}