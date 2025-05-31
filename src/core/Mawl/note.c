#include "raylib.h"
#include "gameplay.h"
#include "note.h"
#include "context.h"
#include "timer.h"
#include <stdio.h>
#include "sfx.h"
#include "linked_list.h"
#include <math.h>


void note_draw(NoteManager *self){
  // Mulai gambar akurasi hanya jika note pertama sampai
  if(self->isFirstHit){
    _drawAccuracy(self);
  }
  // Cek apakah beatmap sudah diload
  if(self->isBeatmapLoaded){
    NodeAddress cur = self->noteHead;
      while (cur != NULL)
      {
        DrawableNote* note = (DrawableNote*)(cur->info);
        if(note->isTrailVisible) _drawNoteTrail(self, *note);
        if(!(note->position.y <= 0) && !note->isHit && !note->isHoldSuccess && !note->isHolding){
          _drawBeatmapNote(self, *note);
        }
        cur = cur->next;
      }
    }

    
  }
  void note_update(NoteManager *self){
    if(self->gp->life <= 0 && self->ctx->app_state == APP_PLAYING){
      self->ctx->app_state = END_OF_THE_GAME;
      self->ctx->is_dead = true;
      StopSelectedTrack(self->ctx);
      _resetNoteManager(self);
      // DrawText("FAIL", GetScreenWidth()/2 - MeasureText("FAIL", 100)/2, GetScreenHeight()/2, 100, BLACK);
      self->gp->life = 100;
    }
    // Mulai timer/countdown untuk memulai game (3 detik)
    if(!self->timer.is_started) {
      // self->gp->gameTime = 0;
      timer_start(&self->timer, 3);
      printf("Mulai timer di:%f\n", self->gp->gameTime);
    }
    
    if(!self->musicTimer.is_started){
      // self->gp->gameTime = 0;
      timer_start(&self->musicTimer, 3 + ms_to_s(self->gp->gameTimeOffset));
    }
    // if(self->timer.is_started) printf("COUNTDOWN: %s\n", time_elapsed(&(self->timer)));
    if(!self->isTrackPlayed && is_timer_end(&(self->musicTimer))){
      printf("Music start time: %f\n", self->gp->gameTime);
      PlaySelectedTrack(self->ctx);
      self->isTrackPlayed = true;
      // self->gp->isPlaying = true;
    }

  if(self->isTrackPlayed){
    if(IsSelectedMusicEnd(self->ctx) ){
      self->ctx->app_state = END_OF_THE_GAME;
      _resetNoteManager(self); // Ensure all note-related states are reset
      self->gp->life = 100;
      return;
    }
  }  
  // Inisialisasi posisi note jika beatmap sudah diload dan timer sudah selesai
    if(!self->isBeatmapLoaded){
      printf("Beatmap loaded at: %.2f\n\n", self->gp->gameTime);
      self->beatmap = GetSelectedMusicBeatmap(self->ctx);
      for (int i = 0; i < self->beatmap.len; i++)
      {
        self->beatmap.items[i].position.y = -999;
        self->beatmap.items[i].isSpawned = false;
        // self->note[i].isHit = false;
      }
      _extractNoteFromBeatmap(self);
        self->isBeatmapLoaded = true;
        
    }
      _updateNotePosition(self);



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
  self->scoreManager = scoreManager;
  self->isBeatmapLoaded = false;
  self->noteHead = NULL;
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
  bool isMissPos = notePos >= padY - 20 && notePos <= padY + self->gp->padSize + 15;
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
  NodeAddress curr = self->noteHead;
  // Dapatkan frametime
  float dt = GetFrameTime();
  while(curr != NULL)
  {
    double elapsed = time_elapsed(&(self->timer));
    DrawableNote *note = (DrawableNote*)(curr->info);
    double to_hit = ms_to_s(note->hit_at_ms);

    // Jika belum waktunya muncul, maka lanjutkan iterasi (skip)
    if((elapsed > to_hit - self->timeToHitPad)){
      // Jika sudah waktunya untuk muncul dan not belum muncul, maka munculkan note;
      if(!note->isSpawned){
        note->position.y = self->ctx->screen_height;
        note->isSpawned = true;
        note->isTrailVisible = true;
        // printf("Note spawned at: %f\n", self->gp->gameTime);
      }
      float note_speed = ((self->ctx->screen_height - 45)/self->timeToHitPad) * dt ; 
      note->position.y -= note_speed;
      if(note->duration_in_ms > 0){
        _noteHoldHitHandler(self, &(*note));
      }else{
        _noteHitHandler(self, &(*note));
      }
    }
      curr = curr->next;
  }
}

void _noteHitHandler(NoteManager* self, DrawableNote *note){
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
void _noteHoldHitHandler(NoteManager* self, DrawableNote *note) {
    double currentTime = self->gp->gameTime + self->gp->gameTimeOffset;
    double startTime = note->hit_at_ms + self->gp->gameTimeOffset;
    double endTime = startTime + note->duration_in_ms;
    static bool isStillHeld = false;
    // printf("start time: %f\nduration: %f\nendtime: %f\n" , startTime, note->duration_in_ms, endTime);

    
    // Jika belum mulai hold
    if(!note->isHolding && !note->isHit){
      if(_isNoteHit(self, *note)){  // sama seperti tap, tapi hanya untuk trigger awal
        note->isHolding = true;
        self->gp->alpha = 255;
        self->isFirstHit = true;
        // printf("current time: %f\n start time: %f\n", currentTime, startTime);
      }
    }

    // Jika sedang hold
    if(note->isHolding && !note->isHit){
      // Cek apakah tombol masih ditekan sesuai arah
      switch(note->direction){
        case NOTE_LEFT:
        isStillHeld = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_D);
        break;
        case NOTE_DOWN:
        isStillHeld = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_J);
        break;
        case NOTE_UP:
        isStillHeld = IsKeyDown(KEY_UP) || IsKeyDown(KEY_F);
        break;
        case NOTE_RIGHT:
        isStillHeld = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_K);
        break;
      }
      
      // Jika tombol dilepas sebelum waktunya
      if(!isStillHeld && currentTime < endTime){
        // printf("YAH LEPAS!\n");
        note->isHit = true;
        self->acc = MISS;
        AddScore(self->scoreManager, self->acc);
        UpdateLife(self->gp, self->acc);
        note->isTrailVisible = false;
        self->gp->alpha = 255;
        return;
      }
      // printf("isStillHeld: %d\n", isStillHeld);

      // Jika hold selesai dengan sukses
      if(isStillHeld && currentTime >= endTime){
        // printf("GACOR KANG!\n");
        note->isHit = true;
        note->isHoldSuccess = true;
        self->acc = PERFECT;
        AddScore(self->scoreManager, self->acc);
        UpdateLife(self->gp, self->acc);
        note->isTrailVisible = false;
        self->gp->alpha = 255;
        printf("Hold success duration: %f\n", startTime - endTime);
        printf("Actual duration: %f\n", note->duration_in_ms);
        printf("Current time: %f\n", currentTime);
      }
    }
    // Cek apakah sudah MISS karena lewat waktu dan belum holding
    if(currentTime > endTime && !note->isHoldSuccess && !note->isHit){
        note->isHit = true;
        self->acc = MISS;
        AddScore(self->scoreManager, self->acc);
        UpdateLife(self->gp, self->acc);
        self->gp->alpha = 255;
        self->isFirstHit = true;
        // printf("EndTime: %f\nCurrentTime: %f\n", endTime, currentTime);
        // printf("YAH LEWAT!\n");
        note->isTrailVisible = false;
        return;
    }
}


void _extractNoteFromBeatmap(NoteManager* self){
  DrawableNote *noteToInsert;
  for (int i = 0; i < self->beatmap.len; i++)
  {
    noteToInsert = (DrawableNote*)malloc(sizeof(DrawableNote));
    noteToInsert->direction = self->beatmap.items[i].direction; 
    noteToInsert->hit_at_ms = self->beatmap.items[i].hit_at_ms + self->gp->gameTimeOffset - 500; // offset waktu untuk hit pad
    noteToInsert->position = self->beatmap.items[i].position; 
    noteToInsert->isHit = false;
    noteToInsert->isSpawned = false;
    noteToInsert->isHolding = false;
    noteToInsert->isHoldSuccess = false;
    noteToInsert->isTrailVisible = false;
    noteToInsert->duration_in_ms = self->beatmap.items[i].duration_in_ms;
    node_append(&(self->noteHead), (void*)noteToInsert);
  }
}

void _resetNoteManager(NoteManager *self) {
  NodeAddress cur = self->noteHead;
  self->gp->timer.is_started = false;
  self->gp->gameTime = 0;
  self->gp->isBackgroundLoaded = false;
  self->isBeatmapLoaded = false;
  self->isTrackPlayed = false;
  self->isFirstHit = false;
  self->acc = PERFECT;
  self->timer.is_started = false;
  self->musicTimer.is_started = false;
  // self->gp->isPlaying = false;
  // SeekMusicStream(GetSelectedTrack(self->ctx).music, 0.1f);
  // self->gp->gameTimeOffset
  while(self->noteHead != NULL) {
    free(self->noteHead->info);
    node_remove_first(&(self->noteHead));
  }
  self->noteHead = NULL;

}

void _drawNoteTrail(NoteManager* self, DrawableNote note) {
    Vector2 position = note.position; // Ini adalah posisi Y kepala note saat ini
    Color trailColor1;
    Color trailColor2;

    // Tentukan X posisi dan warna berdasarkan arah note
    // (Kode switch-case Anda untuk ini sudah benar)
    switch (note.direction) {
        case NOTE_LEFT:
            position.x = self->gp->padPositions[0].x;
            trailColor1 = (Color){0xA6, 0x38, 0xEF, 0xFF};
            trailColor2 = (Color){0x22, 0x72, 0xC7, 0xFF};
            break;
        case NOTE_RIGHT:
            position.x = self->gp->padPositions[3].x;
            trailColor1 = (Color){0xFD, 0xF1, 0x71, 0xFF};
            trailColor2 = (Color){0xF9, 0x84, 0xB9, 0xFF};
            break;
        case NOTE_UP:
            position.x = self->gp->padPositions[1].x;
            trailColor1 = (Color){0xF8, 0x53, 0x51, 0xFF};
            trailColor2 = (Color){0x63, 0x21, 0x79, 0xFF};
            break;
        case NOTE_DOWN:
            position.x = self->gp->padPositions[2].x;
            trailColor1 = (Color){0x4B, 0xF7, 0xFE, 0xFF};
            trailColor2 = (Color){0xAE, 0xAD, 0x3B, 0xFF};
            break;
    }

    if (note.duration_in_ms > 0 && note.isTrailVisible) { // Tambahkan cek isTrailVisible jika perlu
        // Tentukan Y-koordinat garis target/hit secara konsisten
        // Ini harus sama dengan Y yang digunakan untuk menghitung kecepatan di _updateNotePosition
        // Jika _updateNotePosition menggunakan '45.0f' sebagai hit line Y:
        // float target_hit_line_y = 45.0f;
        // Namun, lebih baik menggunakan posisi pad aktual:
        float target_hit_line_y = self->gp->padPositions[0].y + (self->gp->padSize / 2);
        // (Asumsi semua pad di Y yang sama untuk hit. Jika tidak, sesuaikan berdasarkan note.direction)

        // Jarak total yang ditempuh note dari spawn (screen_height) ke target_hit_line_y
        // Ini mengasumsikan note spawn di Y = screen_height dan bergerak ke atas menuju target_hit_line_y
        float travel_distance = self->ctx->screen_height - target_hit_line_y;

        // Kecepatan note dalam piksel per detik (konsisten dengan _updateNotePosition)
        // self->timeToHitPad diinisialisasi sebagai 1.5f (detik)
        float note_speed_pps = travel_distance / self->timeToHitPad;

        // Panjang hold note yang dikoreksi: kecepatan * durasi hold (dalam detik)
        float correctedHoldLength = note_speed_pps * ms_to_s(note.duration_in_ms);

        // Hapus faktor * 0.7 karena perhitungan kecepatan sekarang seharusnya sudah benar
        float holdLength = correctedHoldLength;

        // Pastikan holdLength tidak negatif jika ada masalah kalkulasi (misal note.position.y sudah melewati target)
        if (holdLength < 0) {
            holdLength = 0;
        }

        // Posisi dan dimensi untuk Rectangle trail
        // position.x sudah diset untuk lane yang benar
        // Lebar trail (Anda menggunakan self->gp->padSize / 2)
        float trailWidth = self->gp->padSize / 2;
        // X untuk trail agar di tengah note/pad (sesuai kode Anda)
        float trailX = position.x + self->gp->padSize / 2 - trailWidth / 2;

        // Y untuk trail: Trail memanjang ke bawah dari kepala note
        // (karena note bergerak ke atas, jadi Y trail lebih besar dari Y kepala note)
        // Kepala note ada di note.position.y. Trail dimulai dari bawahnya.
        float trailStartY = note.position.y + self->gp->padSize / 2; // Sesuai kode Anda

        Rectangle holdBody = {
            trailX,
            trailStartY,
            trailWidth,
            holdLength
        };

        // Menggambar trail dengan ScissorMode untuk memotong jika trail melewati batas pad
        // Y untuk ScissorMode adalah Y atas dari pad/receptor
        float scissorY = self->gp->padPositions[0].y + (self->gp->padSize / 2);

        BeginScissorMode((int)holdBody.x, (int)scissorY, (int)holdBody.width, (int)(self->ctx->screen_height - scissorY));
        DrawRectangleGradientV(holdBody.x, holdBody.y, holdBody.width, holdBody.height, trailColor1, trailColor2);
        EndScissorMode();
    }
}