#include "context.h"
#include "selection_menu.h"
#include "scene.h"
#include <stdio.h>
#include "math.h"

float j = 0;

void SelectionMenu_Draw(SelectionMenu *self) {
  float fontsize = 30;
  float padding = 70;
  float textPadding = 10; // Padding dalam kotak background
  int selected = self->ctx->selected_track;

  for (int i = 0; i < self->ctx->tracks.len; i++) {
      int distance = abs(i - selected); // Jarak dari selected_track
      float scaleFactor = 1.0f / (1.0f + 0.3f * distance); // Skala berkurang seiring jarak
      float scale = (i == selected) ? 1.2f : scaleFactor;
      
      Color textColor = (i == selected) ? GOLD : LIGHTGRAY;
      Color bgColor = (i == selected) ? DARKGRAY : GRAY;
      bgColor = Fade(bgColor, 0.8f);

      float r = self->ctx->screen_height / 2;
      float angle = ((i + self->rotationOffset) * PI) / self->ctx->tracks.len;
      
      float yPosition = self->ctx->screen_height / 2 + r * sin(angle);
      float xPosition = self->ctx->screen_width / 2 - self->ctx->screen_width + r * cos(angle);

      Vector2 textSize = MeasureTextEx(GetFontDefault(), self->ctx->tracks.track[i].music_name, fontsize * scale, 2);
      
      DrawCircleLines(self->ctx->screen_width / 2 - self->ctx->screen_width, self->ctx->screen_height / 2, r, BLACK);
      
      DrawRectangle(xPosition - textPadding, yPosition - textPadding, textSize.x + 2 * textPadding < 400 ? 400 : textSize.x + 2 * textPadding, textSize.y + 2 * textPadding * 2, bgColor);
      
      DrawTextEx(self->ctx->font, self->ctx->tracks.track[i].music_name, (Vector2){ xPosition, yPosition }, fontsize * scale, 2, textColor);
      
      DrawTextEx(self->ctx->font, TextFormat("Best Score: %d", self->ctx->tracks.track[i].high_score), (Vector2){ xPosition, yPosition + 20 * scale + textPadding }, 20 * scale, 2, textColor);
      
      DrawRectangle(xPosition, yPosition, 10, 10, RED);
  }
}

void SelectionMenu_Update(SelectionMenu *self) {
  static float time = 0;
  static float delay = 0.35f;

  float maxDelay = 0.01f;
  float minDelay = 0.35f;

  if (IsKeyPressed(KEY_DOWN)) {
    self->ctx->selected_track++;
    if (self->ctx->selected_track >= self->ctx->tracks.len) {
      self->ctx->selected_track = 0;
    } 
  } else if (IsKeyPressed(KEY_UP)) {
    self->ctx->selected_track--;
    if (self->ctx->selected_track < 0) {
      self->ctx->selected_track = self->ctx->tracks.len - 1;
    }
  }
  

  if (IsKeyDown(KEY_DOWN)) {
    time += GetFrameTime();
    if(time >= delay && time >= 0.1){
      self->ctx->selected_track++;
      if (self->ctx->selected_track >= self->ctx->tracks.len) {
        self->ctx->selected_track = 0;
      }
      time = 0;
    }
    
    if (delay >= maxDelay)
    {
      delay -= 0.0075f;
    }
  } else if (IsKeyDown(KEY_UP)) {
    time += GetFrameTime();
    if(time >= delay){
      self->ctx->selected_track--;
      if (self->ctx->selected_track < 0) {
        self->ctx->selected_track = self->ctx->tracks.len - 1;
      }
      time = 0;
    }
    
    if (delay >= maxDelay)
    {
      delay -= 0.0075f;
    }
  } else if (IsKeyPressed(KEY_ENTER)) {
    self->ctx->app_state = APP_PLAYING;
  }
  
  if (delay <= minDelay)
  {
    delay += 0.0025f;
  }
  

  self->targetYOffset = -self->ctx->selected_track * 110;
  self->selectionYOffset += (self->targetYOffset - self->selectionYOffset) * 0.1f;
  
  self->targetRotationOffset = -self->ctx->selected_track;
  self->rotationOffset += (self->targetRotationOffset - self->rotationOffset) * 0.1f;
}

bool SelectionMenu_IsShow(SelectionMenu *self) {
  // untuk menghilangkan warning, hapus ketika akan digunakan
  if(self->ctx->app_state == APP_SELECT){
    return true;
  }
  return false;
}