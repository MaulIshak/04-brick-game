#include "raylib.h"
#include "context.h"
#include "macro.h"
#include "gameplay.h"
#include <string.h>
#include <stdio.h>
#include "timer.h"

static Color secondary = BLACK;
static Color primary = BLACK;

void gp_draw(Gameplay* self){
    Rectangle rec = {
      0,0,self->width, self->ctx->screen_height
    };
    // DrawRectangleGradientEx(rec, PRIMARY_COLOR, SECONDARY_COLOR, SECONDARY_COLOR, PRIMARY_COLOR);

    DrawLine(self->width, 0, self->width, self->ctx->screen_height, BLACK);
    DrawFPS(0,0);
    for (int i = 0; i < LINE_COUNT; i++)
    {
      DrawTextureEx(self->textureToLoad[i], self->padPositions[i],0, 1.5f, (Color){ 240, 240, 240, self->padOpacity[i] });
    }
  
    
  }
  void gp_update(Gameplay* self){
    if(!self->timer.is_started){
      self->startGameTime = GetTime();
      timer_start(&(self->timer), 3);
    }
    if(is_timer_end(&(self->timer))){
      _UpdateGameTime(self);

    }
    // DOWN ARROW (MIDDLE LEFT)
    if(IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_J)||IsGamepadButtonDown(0,GAMEPAD_BUTTON_RIGHT_TRIGGER_1)){
      self->padOpacity[2] = 255;
      // printf("Hit time: %f\n", self->gameTime);
    }else{
      self->padOpacity[2] = 100;
    }

    // LEFT ARROW (LEFT)
    if(IsKeyDown(KEY_LEFT) ||IsKeyDown(KEY_D)|| IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_TRIGGER_2)){
      self->padOpacity[0] = 255;
    }else{
      self->padOpacity[0] = 100;
      
    }

    // UP ARROW (MIDDLE RIGHT)
    if(IsKeyDown(KEY_UP) ||IsKeyDown(KEY_F)|| IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_TRIGGER_1)){
      self->padOpacity[1] = 255;
    }else{
      self->padOpacity[1] = 100;
      
    }

    // RIGHT ARROW (RIGHT)
    if(IsKeyDown(KEY_RIGHT) ||IsKeyDown(KEY_K) ||IsGamepadButtonDown(0,GAMEPAD_BUTTON_RIGHT_TRIGGER_2)){
      self->padOpacity[3] = 255;
    }else{
      self->padOpacity[3] = 100;
      
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
    "resources/texture/Arrow_YangAdaPutihnya-03.png",
    "resources/texture/Arrow_YangAdaPutihnya-04.png",
    "resources/texture/Arrow_YangAdaPutihnya-02.png",
    "resources/texture/Arrow_YangAdaPutihnya-01.png"
  };
  gameplay->ctx = ctx;
  gameplay->width = 400;
  memcpy(gameplay->texturePaths, textureSources, sizeof(textureSources));
  for (int i = 0; i < LINE_COUNT; i++)
  {
    gameplay->padOpacity[i] = 100;
    gameplay->padPositions[i].x = gameplay->ctx->screen_width/6 * i +10;
    gameplay->padPositions[i].y = 48;
  }
  gameplay->gameTime = 0;
  _LoadNoteTexture(gameplay);
  gameplay->timer = (Timer){false, 0,0};
  gameplay->gameTimeOffset = 2000;
}

void _LoadNoteTexture(Gameplay*self){
  for (int i = 0; i < TEXTURE_COUNT; i++)
  {
    self->textureToLoad[i] = LoadTexture(self->texturePaths[i]);
  }
  
}

void _UpdateGameTime(Gameplay* self){
  float currentTime = GetTime();
  self->gameTime =s_to_ms(currentTime - self->startGameTime - 3);
}