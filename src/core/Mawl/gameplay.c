#include "raylib.h"
#include "context.h"
#include "note.h"
#include "macro.h"
#include "gameplay.h"

void gp_draw(Gameplay* self){
  // for (int i = 0; i < TEXTURE_COUNT; i++)
  // {
    // }
    Color primary = (Color){ 240, 240, 240, 255 };
    
    // Color secondary = (Color){ 190, 190, 190, 255 };
    Rectangle rec = {
      0,0,self->width, self->ctx->screen_height
    };
    // DrawRectangleGradientEx(rec,primary,secondary, secondary, primary);
    DrawRectangleRec(rec,primary);
    DrawLine(self->width, 0, self->width, self->ctx->screen_height, BLACK);
    DrawTextureEx(self->textureToLoad[0], (Vector2){120, 110},180.0f, .2f, (Color){ 240, 240, 240, self->lineNotesOpacity[0] });
    DrawTextureEx(self->textureToLoad[0], (Vector2){220, 0},90.0f, .2f, (Color){ 240, 240, 240, self->lineNotesOpacity[1] });
    DrawTextureEx(self->textureToLoad[0], (Vector2){200, 110},270.0f, .2f, (Color){ 240, 240, 240, self->lineNotesOpacity[2] });
    DrawTextureEx(self->textureToLoad[0], (Vector2){300, 0},.0f, .2f, (Color){ 240, 240, 240, self->lineNotesOpacity[3] });
    
  }
  void gp_update(Gameplay* self){

    // UP ARROW (MIDDLE RIGHT)
    if(IsKeyDown(KEY_UP) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_RIGHT_TRIGGER_1)){
      self->lineNotesOpacity[2] = 255;
    }else{
      self->lineNotesOpacity[2] = 100;
    }

    // LEFT ARROW (LEFT)
    if(IsKeyDown(KEY_LEFT) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_TRIGGER_2)){
      self->lineNotesOpacity[0] = 255;
    }else{
      self->lineNotesOpacity[0] = 100;
      
    }

    // DOWN ARROW (MIDDLE LEFT)
    if(IsKeyDown(KEY_DOWN) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_TRIGGER_1)){
      self->lineNotesOpacity[1] = 255;
    }else{
      self->lineNotesOpacity[1] = 100;
      
    }

    // RIGHT ARROW (RIGHT)
    if(IsKeyDown(KEY_RIGHT) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_RIGHT_TRIGGER_2)){
      self->lineNotesOpacity[3] = 255;
    }else{
      self->lineNotesOpacity[3] = 100;
      
    }
  }

bool gp_isShow(Gameplay* self){
  if(self->ctx->app_state == APP_PLAYING){
    return true;
  }
  return false;
}

void InitGameplay(Gameplay *gameplay, AppContext *ctx){
  gameplay->ctx = ctx;
  gameplay->width = 430;
  gameplay->texturePath[0] = "src/core/Mawl/arrow.png";
  for (int i = 0; i < LINE_COUNT; i++)
  {
    gameplay->lineNotesOpacity[i] = 100;
  }
  
  _LoadNoteTexture(gameplay);
}

void _LoadNoteTexture(Gameplay*self){
  for (int i = 0; i < TEXTURE_COUNT; i++)
  {
    self->textureToLoad[i] = LoadTexture(self->texturePath[i]);
  }
  
}