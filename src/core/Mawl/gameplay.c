#include "raylib.h"
#include "context.h"
#include "macro.h"
#include "gameplay.h"
#include <string.h>
#include <stdio.h>
#include "timer.h"
#include "progress_bar.h"
#include "sfx.h"
#include "score.h"


static Color secondary = BLACK;
static Color primary = BLACK;
// Texture2D meledak;
// Vector2 position = { 350.0f, 280.0f };
// Rectangle frameRec;
// int currentFrame = 0;
// int framesCounter = 0;
// int framesSpeed = 12;  
Texture2D bg;

void gp_draw(Gameplay* self){
  // Variable lokal
    Rectangle rec = {
      0,0,self->width, self->ctx->screen_height
    };
    Rectangle rec2 = {
      self->padPositions[0].x, 0,self->padPositions[3].x + 10, self->ctx->screen_height
    };
    Rectangle rec3 = {
      self->padPositions[0].x, self->padPositions[0].y, self->padSize , self->padSize
    };
    char* control[LINE_COUNT]= {"D", "F", "J", "K"};


    // Draw

    // DrawRectangleGradientEx(rec, PRIMARY_COLOR, SECONDARY_COLOR, SECONDARY_COLOR, PRIMARY_COLOR);
    DrawTexture(bg,0,0,WHITE);   
    DrawRectangleRec(rec, Fade(WHITE, self->alpha/255 - 0.7f));
    DrawRectangleRec(rec2, Fade(BLACK, .5f));
    // _drawAccZone(self);
    DrawLine(self->width, 0, self->width, self->ctx->screen_height, BLACK);
    for (int i = 0; i < LINE_COUNT; i++)
    {
      DrawTextureEx(self->textureToLoad[i], self->padPositions[i],0, .16f, (Color){ 240, 240, 240, self->padOpacity[i] });
      DrawTextEx(self->ctx->font, control[i], (Vector2){self->padPositions[i].x + self->padSize/2 - 7, self->padPositions[i].y - 30}, 40, 1,WHITE);
    }

  
    // Progress Bar
    DrawProgressBar(&self->progressBar);

    // Life Bar
    _drawLifeBar(self);
    // DrawTexture(meledak, 0, 0, WHITE);
    // DrawTextureRec(meledak, frameRec, position, WHITE);  // Draw part of the texture

    
  }
  void gp_update(Gameplay* self){
    _updateLifeBar(self);
    // -- Ini kode untuk animasi sprite, tetapi karena tidak berjalan sesuai harapan, jadi tidak digunakan dulu --
    // printf("Time: %f\n", self->gameTime);
    // Tes sprite animation
    // framesCounter++;

    // if (framesCounter >= (60/framesSpeed))
    // {
    //     framesCounter = 0;
    //     currentFrame++;

    //     if (currentFrame > 7) currentFrame = 0;

        // frameRec.x = (float)7*(float)meledak.width/8;
        // }
        // static float time;
        // time += GetFrameTime();
        // if(time > .5f){
        //   currentFrame++;
        //   time = 0;
        //   frameRec.x = (float)currentFrame*(float)meledak.width/7;

        // }

    // Update Gameplay
    if(!self->isBackgroundLoaded){
      bg = _getRandomBg(self);
      self->isBackgroundLoaded = true;
    }
    // Mulai waktu game
    if(!self->timer.is_started){
      // printf("START GAME TIME!\n");
      self->startGameTime = GetTime();
      timer_start(&(self->timer), 3);
    }
    if(is_timer_end(&(self->timer))){
      _UpdateGameTime(self);
    }
    UpdateProgressBar(&self->progressBar, self);

    // DOWN ARROW (MIDDLE LEFT)
    if(IsKeyDown(KEY_J) || IsKeyDown(KEY_J)||IsGamepadButtonDown(0,GAMEPAD_BUTTON_RIGHT_TRIGGER_1)){
      self->textureToLoad[2] = self->activeTextureToLoad[3];
    }else{
      self->textureToLoad[2] = self->passiveTextureToLoad[3];
    }
    
    // LEFT ARROW (LEFT)
    if(IsKeyDown(KEY_D) ||IsKeyDown(KEY_D)|| IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_TRIGGER_2)){
      self->textureToLoad[0] = self->activeTextureToLoad[0];
    }else{
      self->textureToLoad[0] = self->passiveTextureToLoad[0];
      
    }

    // UP ARROW (MIDDLE RIGHT)
    if(IsKeyDown(KEY_F) ||IsKeyDown(KEY_F)|| IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_TRIGGER_1)){
      self->textureToLoad[1] = self->activeTextureToLoad[1];
    }else{
      self->textureToLoad[1] = self->passiveTextureToLoad[1];
      
    }
    
    // RIGHT ARROW (RIGHT)
    if(IsKeyDown(KEY_K) ||IsKeyDown(KEY_K) ||IsGamepadButtonDown(0,GAMEPAD_BUTTON_RIGHT_TRIGGER_2)){
      self->textureToLoad[3] = self->activeTextureToLoad[2];
    }else{
      self->textureToLoad[3] = self->passiveTextureToLoad[2];
    }

    if(IsKeyPressed(KEY_K) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_F) || IsKeyPressed(KEY_J)){
      PlayMissSfx();
    }

    if(self->life >= 100){
      self->life = 100;
    }

  }

bool gp_isShow(Gameplay* self){
  if(self->ctx->app_state == APP_PLAYING){
    return true;
  }
  return false;
}

void InitGameplay(Gameplay *gameplay, AppContext *ctx){
  char *textureSources [LINE_COUNT] ={
    "resources/texture/Pad-01.png",
    "resources/texture/Pad-02.png",
    "resources/texture/Pad-03.png",
    "resources/texture/Pad-04.png"
  };
  char *newTextureSource [LINE_COUNT]={
    "resources/texture/Pad_Active-01.png",
    "resources/texture/Pad_Active-02.png",
    "resources/texture/Pad_Active-04.png",
    "resources/texture/Pad_Active-03.png"
  };

  gameplay->ctx = ctx;
  gameplay->width = gameplay->ctx->screen_width;
  memcpy(gameplay->texturePaths, textureSources, sizeof(textureSources));
  memcpy(gameplay->textureActivePaths, newTextureSource, sizeof(newTextureSource));
  for (int i = 0; i < LINE_COUNT; i++)
  {
    gameplay->padOpacity[i] = 255;
    gameplay->padPositions[i].x = gameplay->ctx->screen_width/6 * i+ gameplay->ctx->screen_width/8;
    gameplay->padPositions[i].y = 48;
  }
  gameplay->gameTime = 0;
  _LoadNoteTexture(gameplay);
  gameplay->timer = (Timer){false, 0,0};
  gameplay->gameTimeOffset = 2000;

  InitProgressBar(&gameplay->progressBar, 0, 0, gameplay->width, 10, SECONDARY_COLOR);
  gameplay->padSize = 512*0.16f +5;
  gameplay->alpha = 0;
  gameplay->background[0] = LoadTexture("resources/background/Background-01.png");
  gameplay->background[1] = LoadTexture("resources/background/Background-02.png");
  gameplay->background[2] = LoadTexture("resources/background/Background-03.png");
  gameplay->maxLife = 100;
  gameplay->life = gameplay->maxLife;
  gameplay->lifeBar.height = 20;
  gameplay->lifeBar.width = 80;

  gameplay->lifeBar.outlineRec = (Rectangle){.height=gameplay->lifeBar.height, .width= gameplay->lifeBar.width, .x=GetScreenWidth()-GetScreenWidth()/6, .y=GetScreenHeight()/4};
  gameplay->lifeBar.lifeRec = (Rectangle){.height=gameplay->lifeBar.height, .width= gameplay->lifeBar.width, .x=GetScreenWidth()-GetScreenWidth()/6, .y=GetScreenHeight()/4};
  // meledak = LoadTexture("resources/texture/Meledak-2.png");
  // frameRec = (Rectangle){ 0.0f, 0.0f, (float)meledak.width/8, (float)meledak.height };
  gameplay->isBackgroundLoaded = false;
}

void _LoadNoteTexture(Gameplay*self){
  for (int i = 0; i < TEXTURE_COUNT; i++)
  {
    self->passiveTextureToLoad[i] = LoadTexture(self->texturePaths[i]);
    self->activeTextureToLoad[i] = LoadTexture(self->textureActivePaths[i]);
  }
  for (int i = 0; i < TEXTURE_COUNT; i++)
  {
    /* code */
    self->textureToLoad[i] =  self->passiveTextureToLoad[i];
  }
  
  
}

void _UpdateGameTime(Gameplay* self){
  float currentTime = GetTime();
  self->gameTime =s_to_ms(currentTime - self->startGameTime - 3);
  // printf("%.2f\n", self->gameTime);
}

void _drawAccZone(Gameplay* self){
  Rectangle recPerfect = {
    self->padPositions[0].x, self->padPositions[0].y + 20, self->padPositions[3].x + 10 , self->padSize - 20*2
  };
  Rectangle recGood = {
    self->padPositions[0].x, self->padPositions[0].y, self->padPositions[3].x + 10 , self->padSize
  };
  Rectangle recMiss = {
    self->padPositions[0].x, self->padPositions[0].y - 20, self->padPositions[3].x + 10 , self->padSize + 20 + 20
  };

  DrawRectangleRec(recPerfect, Fade(GREEN, 1));
  DrawRectangleRec(recGood, Fade(YELLOW, 0.2f));
  DrawRectangleRec(recMiss, Fade(RED, 0.2f));
}

Texture2D _getRandomBg(Gameplay* self){
  int rand = GetRandomValue(0, 2);
  return self->background[rand];
}


void _drawLifeBar(Gameplay* self){
  DrawText("Life",self->lifeBar.outlineRec.x, self->lifeBar.outlineRec.y - 30, 30, BLACK);
  DrawRectangleRec(self->lifeBar.outlineRec, BLACK);
  DrawRectangleRec(self->lifeBar.lifeRec, GREEN);
}

void _updateLifeBar(Gameplay* self){
  self->lifeBar.lifeRec.width = (self->life/self->maxLife) * self->lifeBar.width;
}

void UpdateLife(Gameplay *self, Accuracy acc){
  if(acc == MISS){
    self->life -= 10;
  }else if(acc == PERFECT){
    self->life += 5;
  }
}