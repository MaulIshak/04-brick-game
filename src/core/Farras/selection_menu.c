#include "context.h"
#include "selection_menu.h"
#include "scene.h"
#include <stdio.h>
#include "math.h"
#include "flying_object.h"

float textOffsetX = 0;
int indexMove = 0;

float flashAlpha = 0.0f;  // Transparansi efek kilau
int lastSelected = -1;     // Menyimpan track yang terakhir dipilih

void SelectionMenu_Draw(SelectionMenu *self) {
    FlyingObject_Draw(&self->flying_objects);


    int boxWidth = 400;
    float fontsize = 30;
    float textPadding = 10;
    int selected = self->ctx->selected_track;

    // Cek jika ada perubahan item yang dipilih
    if (selected != lastSelected) {
        flashAlpha = 1.0f;  // Reset efek kilau
        lastSelected = selected;
    }

    ClearBackground(BLACK);
    DrawCircleGradient(self->ctx->screen_width / 2, self->ctx->screen_height / 2, self->ctx->screen_height, (Color){254, 250, 148, 200}, (Color){255, 147, 98, 200});
    DrawCircleGradient(self->ctx->screen_width / 2 - 100, self->ctx->screen_height / 2 + 500, self->ctx->screen_height, (Color){255, 147, 98, 200}, (Color){255, 92, 93, 200});
    DrawCircleGradient(self->ctx->screen_width / 2 - 200, self->ctx->screen_height / 2 + 600, self->ctx->screen_height, (Color){255, 92, 93, 200}, (Color){128, 69, 255, 200});
    DrawCircleGradient(self->ctx->screen_width / 2 - 300, self->ctx->screen_height / 2 + 700, self->ctx->screen_height, (Color){128, 69, 255, 200}, (Color){57, 43, 214, 200});
    DrawCircleGradient(self->ctx->screen_width / 2 - 400, self->ctx->screen_height / 2 + 800, self->ctx->screen_height, (Color){57, 43, 214, 200}, (Color){24, 29, 149, 200});
    DrawCircle(self->ctx->screen_width / 2 - 500, self->ctx->screen_height / 2 + 900, self->ctx->screen_height, (Color){24, 29, 149, 200});    

    for (int i = 0 + indexMove; i < self->ctx->tracks.len + indexMove; i++) {
        int distance = abs(i - selected);
        float scaleFactor = 1.2f / (1.0f + 0.2f * distance);
        float scale = (i == selected) ? 1.2f : scaleFactor;

        Color textColor = (i == selected) ? GOLD : LIGHTGRAY;
        Color bgColor = (i == selected) ? DARKGRAY : GRAY;
        bgColor = Fade(bgColor, 0.8f);

        float r = self->ctx->screen_height / 2;
        float angle = ((i + self->rotationOffset) * (PI * 2)) / MAX_MUSIC_SHOWING;
        
        float yPosition = self->ctx->screen_height / 2 + r * sin(angle);
        float xPosition = self->ctx->screen_width / 2 - self->ctx->screen_width + r * cos(angle);

        Vector2 textVector = MeasureTextEx(self->ctx->font, self->ctx->tracks.track[i].music_name, fontsize * scale, 2);

        DrawRectangleGradientH(xPosition - textPadding, yPosition - textVector.y + textPadding / 2,
            boxWidth + textPadding, textVector.y * 2, bgColor, Fade(bgColor, 0.0f));

        if (i == selected && flashAlpha > 0.0f) {
            DrawRectangle(xPosition - textPadding, yPosition - textVector.y + textPadding / 2,
                boxWidth + textPadding, textVector.y * 2, Fade(WHITE, flashAlpha));
        }

        if (textVector.x > boxWidth) {
            textOffsetX += 50 * GetFrameTime();
            if (textOffsetX > textVector.x) {
                textOffsetX = -boxWidth;
            }
        }

        BeginScissorMode(xPosition, yPosition - textVector.y + textPadding / 2, boxWidth, textVector.y + textPadding);
        DrawTextEx(self->ctx->font, self->ctx->tracks.track[i].music_name, (Vector2){ xPosition - (textVector.x > boxWidth ? textOffsetX : 0), yPosition - textVector.y + textPadding }, fontsize * scale, 2, textColor);
        EndScissorMode();

        DrawTextEx(self->ctx->font, TextFormat("Best Score: %d", self->ctx->tracks.track[i].high_score),
            (Vector2){ xPosition, yPosition + (textPadding) / 2 }, (fontsize - 10) * scale, 2, textColor);
    }

    if (flashAlpha > 0.0f) {
        flashAlpha -= 0.03f;  
        if (flashAlpha < 0.0f) flashAlpha = 0.0f;
    }

    
    DrawTexture(self->background, 0, 0, WHITE);
}


void SelectionMenu_Update(SelectionMenu *self) {
  FlyingObject_Update(&self->flying_objects, self->ctx);

  static float time = 0;
  static float delay = 0.35f;
  static bool keyHeld = false;

  const float maxDelay = 0.01f;
  const float minDelay = 0.35f;
  const float delayReduction = 0.0075f;
  const float delayRecovery = 0.0025f;
  const float frameThreshold = 0.1f;

  if (IsKeyPressed(KEY_DOWN)) {
    if (self->ctx->tracks.len > MAX_MUSIC_SHOWING) {
      indexMove++;
      if (indexMove > self->ctx->tracks.len - MAX_MUSIC_SHOWING) {
        indexMove = 0;
      }
    }
    
    textOffsetX = 0;
      self->ctx->selected_track = (self->ctx->selected_track + 1) % self->ctx->tracks.len;
      keyHeld = true;
      time = 0;
  } else if (IsKeyPressed(KEY_UP)) {
    if (self->ctx->tracks.len > MAX_MUSIC_SHOWING) {
        indexMove--;
        if (indexMove > self->ctx->tracks.len - MAX_MUSIC_SHOWING) {
          indexMove = 0;
        }
      }

    textOffsetX = 0;
      self->ctx->selected_track = (self->ctx->selected_track - 1 + self->ctx->tracks.len) % self->ctx->tracks.len;
      keyHeld = true;
      time = 0;
  }
  
  if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_UP)) {
    textOffsetX = 0;

      time += GetFrameTime();
      if (time >= delay) {
          if (!keyHeld || time >= frameThreshold) {
              if (IsKeyDown(KEY_DOWN)) {
                if (self->ctx->tracks.len > MAX_MUSIC_SHOWING) {
                    indexMove++;
                    if (indexMove > self->ctx->tracks.len - MAX_MUSIC_SHOWING) {
                      indexMove = 0;
                    }
                  }
                  self->ctx->selected_track = (self->ctx->selected_track + 1) % self->ctx->tracks.len;
              } else if (IsKeyDown(KEY_UP)) {

                if (self->ctx->tracks.len > MAX_MUSIC_SHOWING) {
                    indexMove--;
                    if (indexMove > self->ctx->tracks.len - MAX_MUSIC_SHOWING) {
                      indexMove = 0;
                    }
                  }

                  self->ctx->selected_track = (self->ctx->selected_track - 1 + self->ctx->tracks.len) % self->ctx->tracks.len;
              }
              time = 0;
              keyHeld = true;
          }
      }
      
      if (delay > maxDelay) {
          delay -= delayReduction;
      }
  } else {
      if (delay < minDelay) {
          delay += delayRecovery;
      }
      keyHeld = false;
  }

  if (IsKeyPressed(KEY_ENTER)) {
      self->ctx->app_state = APP_PLAYING;
  }

  self->targetYOffset = -self->ctx->selected_track * 110;
  self->selectionYOffset += (self->targetYOffset - self->selectionYOffset) * 0.1f;

  self->targetRotationOffset = -self->ctx->selected_track;
  self->rotationOffset += (self->targetRotationOffset - self->rotationOffset) * 0.1f;
}

bool SelectionMenu_IsShow(SelectionMenu *self) {
  if(self->ctx->app_state == APP_SELECT){
    self->isShow = true;
    return true;
  }
  
  self->isShow = false;
  return false;
}

void InitSelectionMenu(SelectionMenu *self, AppContext *ctx) {
    self->ctx = ctx;

    self->rotationOffset = 0;
    self->targetRotationOffset = 0;

    self->selectionYOffset = 0;
    self->targetYOffset = 0;

    self->isShow = false;

    self->flying_objects = FlyingObject_Create(ctx);

    self->background = LoadTexture("resources/background/backgroundEmpty.png");
}