#include "context.h"
#include "selection_menu.h"
#include "scene.h"

void SelectionMenu_Draw(SelectionMenu *self) {
  for (int i = 0; i < self->ctx->tracks.len; i++) {
      Color color = (i == self->ctx->selected_track) ? RED : WHITE;
      DrawText(self->ctx->tracks.track[i].music_name, GetScreenWidth() / 2 - 100, 100 + i * 30, 20, color);
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
}

bool SelectionMenu_IsShow(SelectionMenu *self){
  // untuk menghilangkan warning, hapus ketika akan digunakan
  if(self->ctx->app_state == APP_SELECT){
    return true;
  }
  return false;
}