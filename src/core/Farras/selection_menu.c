#include "context.h"
#include "selection_menu.h"
#include "scene.h"
#include <stdio.h>

void SelectionMenu_Draw(SelectionMenu *self) {
  float fontsize = 30;
  float padding = 70;
  float textPadding = 10; // Padding dalam kotak background

  for (int i = 0; i < self->ctx->tracks.len; i++) {
      float scale = (i == self->ctx->selected_track) ? 1.2f : 1.0f;
      Color textColor = (i == self->ctx->selected_track) ? GOLD : LIGHTGRAY;
      Color bgColor = (i == self->ctx->selected_track) ? DARKGRAY : GRAY;

      float yPosition = self->ctx->screen_height / 2 + (i * (fontsize + padding)) + self->selectionYOffset;
      float xPosition = self->ctx->screen_width / 2 - 200;

      // Ukuran teks
      Vector2 textSize = MeasureTextEx(GetFontDefault(), self->ctx->tracks.track[i].music_name, fontsize * scale, 2);

      DrawRectangle(xPosition - textPadding, yPosition - textPadding, textSize.x + 2 * textPadding < 400 ? 400 : textSize.x + 2 * textPadding, textSize.y + 2 * textPadding * 2, bgColor);
      
      DrawTextEx(GetFontDefault(), self->ctx->tracks.track[i].music_name, (Vector2){ xPosition, yPosition }, fontsize * scale, 2, textColor);

      DrawTextEx(GetFontDefault(), TextFormat("Best Score: %d", self->ctx->tracks.track[i].high_score), (Vector2){ xPosition, yPosition + 20 + textPadding }, 20 * scale, 2, textColor);

      // DrawTextEx(GetFontDefault(), TextFormat("Accuracy: %d", self->ctx->tracks.track[i].accuracy), (Vector2){ xPosition, yPosition + 20 + textPadding }, 20 * scale, 2, textColor);
  }
}

void SelectionMenu_Update(SelectionMenu *self) {
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
  } else if (IsKeyPressed(KEY_ENTER)) {
      self->ctx->app_state = APP_PLAYING;
  }

  self->targetYOffset = -self->ctx->selected_track * 110;
  self->selectionYOffset += (self->targetYOffset - self->selectionYOffset) * 0.1f;
}

bool SelectionMenu_IsShow(SelectionMenu *self){
  // untuk menghilangkan warning, hapus ketika akan digunakan
  if(self->ctx->app_state == APP_SELECT){
    return true;
  }
  return false;
}