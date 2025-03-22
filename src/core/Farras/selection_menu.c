#include "context.h"
#include "selection_menu.h"
#include "scene.h"
#include <stdio.h>
#include "math.h"
#include "flying_object.h"

int indexMove = 0;
float textOffsetX = 0;

void SelectionMenu_Draw(SelectionMenu *self)
{
  ClearBackground(WHITE);

  DrawCircleGradient(self->ctx->screen_width / 2, self->ctx->screen_height / 2, self->ctx->screen_height, (Color){254, 250, 148, 220}, (Color){255, 147, 98, 220});
  DrawCircleGradient(self->ctx->screen_width / 2 - 100, self->ctx->screen_height / 2 + 500, self->ctx->screen_height, (Color){255, 147, 98, 220}, (Color){255, 92, 93, 220});
  DrawCircleGradient(self->ctx->screen_width / 2 - 200, self->ctx->screen_height / 2 + 600, self->ctx->screen_height, (Color){255, 92, 93, 220}, (Color){128, 69, 255, 220});
  DrawCircleGradient(self->ctx->screen_width / 2 - 300, self->ctx->screen_height / 2 + 700, self->ctx->screen_height, (Color){128, 69, 255, 220}, (Color){57, 43, 214, 220});
  DrawCircleGradient(self->ctx->screen_width / 2 - 400, self->ctx->screen_height / 2 + 800, self->ctx->screen_height, (Color){57, 43, 214, 220}, (Color){24, 29, 149, 220});
  DrawCircle(self->ctx->screen_width / 2 - 500, self->ctx->screen_height / 2 + 900, self->ctx->screen_height, (Color){24, 29, 149, 220});

  FlyingObject_Draw(&self->flying_objects);

  SelectionMenuHelper_Draw(self);

  SelectionMenuItem_Draw(self);
}

void SelectionMenu_Update(SelectionMenu *self)
{
  FlyingObject_Update(&self->flying_objects, self->ctx);

  PressToAction(self);

  PreviewMusic(self);

  self->targetYOffset = -self->ctx->selected_track * 110;
  self->selectionYOffset += (self->targetYOffset - self->selectionYOffset) * 0.1f;

  self->targetRotationOffset = -self->ctx->selected_track;
  self->rotationOffset += (self->targetRotationOffset - self->rotationOffset) * 0.1f;
}

void SelectionMenuItem_Draw(SelectionMenu *self)
{
  static int lastSelected = -1;
  static float flashAlpha = 0.0f;

  int boxWidth = 400;
  float fontsize = 30;
  float textPadding = 10;
  int selected = self->ctx->selected_track;
  for (int i = 0 + indexMove; i < self->ctx->tracks.len + indexMove; i++)
  {
    int distance = abs(i - selected);
    float scaleFactor = 1.2f / (1.0f + 0.2f * distance);
    float scale = (i == selected) ? 1.2f : scaleFactor;

    Color textColor = (i == selected) ? GOLD : LIGHTGRAY;
    Color bgColor = (i == selected) ? RAYWHITE : GRAY;
    bgColor = (i == selected) ? bgColor : Fade(bgColor, 0.8f);

    float r = self->ctx->screen_height / 2;
    float angle = ((i + self->rotationOffset) * (PI * 2)) / MAX_MUSIC_SHOWING;

    float yPosition = self->ctx->screen_height / 2 + r * sin(angle);
    float xPosition = self->ctx->screen_width / 2 - self->ctx->screen_width + r * cos(angle);

    Vector2 textVector = MeasureTextEx(self->ctx->font, self->ctx->tracks.track[i].music_name, fontsize * scale, 2);

    if (i == selected) {
      DrawRectangleGradientH(xPosition - textPadding, yPosition - textVector.y + textPadding / 2, boxWidth + textPadding, textVector.y * 2, bgColor, Fade(bgColor, 1.0f));
    } else {
      DrawRectangleGradientH(xPosition - textPadding, yPosition - textVector.y + textPadding / 2, boxWidth + textPadding, textVector.y * 2, bgColor, Fade(bgColor, 0.0f));
    }

    if (i == selected && flashAlpha > 0.0f) {
      DrawRectangle(xPosition - textPadding, yPosition - textVector.y + textPadding / 2, boxWidth + textPadding, textVector.y * 2, Fade(WHITE, flashAlpha));
    }

    if (selected != lastSelected) {
      flashAlpha = 1.0f;
      lastSelected = selected;
    }

    if (textVector.x > boxWidth) {
      textOffsetX += 50 * GetFrameTime();
      if (textOffsetX > textVector.x) {
        textOffsetX = -boxWidth;
      }
    }

    BeginScissorMode(xPosition, yPosition - textVector.y + textPadding / 2, boxWidth, textVector.y + textPadding);
    DrawTextEx(self->ctx->font, self->ctx->tracks.track[i].music_name, (Vector2){xPosition - (textVector.x > boxWidth ? textOffsetX : 0), yPosition - textVector.y + textPadding}, fontsize * scale, 2, textColor);
    EndScissorMode();

    DrawTextEx(self->ctx->font, TextFormat("Best Score: %d", self->ctx->tracks.track[i].high_score),
               (Vector2){xPosition, yPosition + (textPadding) / 2}, (fontsize - 10) * scale, 2, textColor);
  }

  if (flashAlpha > 0.0f) {
    flashAlpha -= 0.03f;
    if (flashAlpha < 0.0f)
      flashAlpha = 0.0f;
  }
}

void buttonDraw(Font font, Vector2 position, char *text, float fontSize, float spacing, Color textColor, Color bgColor, Color borderColor) {
  Vector2 textVector = MeasureTextEx(font, text, fontSize, spacing);

  float xPosition = position.x - textVector.x / 2;
  float yPosition = position.y - textVector.y / 2;

  DrawRectangleRoundedLinesEx((Rectangle){xPosition - 5, yPosition, textVector.x + 10, textVector.y}, 0.4f, 1, 3.5f, borderColor);
  DrawRectangleRounded((Rectangle){xPosition - 5, yPosition, textVector.x + 10, textVector.y}, 0.3f, 1, bgColor);
  DrawTextEx(font, text, (Vector2){xPosition, yPosition}, fontSize, spacing, textColor);
}

void SelectionMenuHelper_Draw(SelectionMenu *self) {
  Font font = self->ctx->font;
  float screenWidth = self->ctx->screen_width;
  float screenHeight = self->ctx->screen_height;

  buttonDraw(font, (Vector2){screenWidth - 100, screenHeight - 30}, "Arrow Up/Down", 22, 2, WHITE, DARKGRAY, GRAY);
  buttonDraw(font, (Vector2){screenWidth - 250, screenHeight - 30}, "(Press To Select)", 18, 1, WHITE, BLANK, BLANK);

  buttonDraw(font, (Vector2){screenWidth - 55, screenHeight - 60}, "Enter", 22, 2, ORANGE, YELLOW, GOLD);
  buttonDraw(font, (Vector2){screenWidth - 155, screenHeight - 60}, "(Press To Play)", 18, 1, WHITE, BLANK, BLANK);
}


void PreviewMusic(SelectionMenu *self) {
  static bool isCanPlay = false;
  static float time = 0.0f;
  static float volume = 0.0f;
  if (IsSelectedMusicEnd(self->ctx)) {
    time += GetFrameTime();
    if (time > 1.5f) {
      isCanPlay = true;
      time = 0.0f;
    }
  }

  if (!self->ctx->is_music_playing || isCanPlay)
  {
    isCanPlay = false;
    volume = 0.0f;
    PlaySelectedTrack(self->ctx);
    SeekMusicStream(self->ctx->tracks.track[self->ctx->selected_track].music, 10);
  }

  if (self->ctx->is_music_playing && volume < 1.0f)
  {
    volume += 0.01f;
    SetMusicVolume(self->ctx->tracks.track[self->ctx->selected_track].music, volume);
  }
}

void PressToAction(SelectionMenu *self)
{
  static float time = 0;
  static float delay = 0.35f;
  static bool keyHeld = false;

  float maxDelay = 0.01f;
  float minDelay = 0.35f;
  float delayReduction = 0.0075f;
  float delayRecovery = 0.0025f;
  float frameThreshold = 0.1f;

  if (IsKeyPressed(KEY_DOWN))
  {
    if (self->ctx->tracks.len > MAX_MUSIC_SHOWING)
    {
      indexMove++;
      if (indexMove > self->ctx->tracks.len - MAX_MUSIC_SHOWING)
      {
        indexMove = 0;
      }
    }

    textOffsetX = 0;
    self->ctx->selected_track = (self->ctx->selected_track + 1) % self->ctx->tracks.len;
    keyHeld = true;
    time = 0;
    StopSelectedTrack(self->ctx);
  }
  else if (IsKeyPressed(KEY_UP))
  {
    if (self->ctx->tracks.len > MAX_MUSIC_SHOWING)
    {
      indexMove--;
      if (indexMove > self->ctx->tracks.len - MAX_MUSIC_SHOWING)
      {
        indexMove = 0;
      }
    }

    textOffsetX = 0;
    self->ctx->selected_track = (self->ctx->selected_track - 1 + self->ctx->tracks.len) % self->ctx->tracks.len;
    keyHeld = true;
    time = 0;
    StopSelectedTrack(self->ctx);
  }

  if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_UP))
  {
    textOffsetX = 0;

    time += GetFrameTime();
    if (time >= delay)
    {
      if (!keyHeld || time >= frameThreshold)
      {
        if (IsKeyDown(KEY_DOWN))
        {
          if (self->ctx->tracks.len > MAX_MUSIC_SHOWING)
          {
            indexMove++;
            if (indexMove > self->ctx->tracks.len - MAX_MUSIC_SHOWING)
            {
              indexMove = 0;
            }
          }
          self->ctx->selected_track = (self->ctx->selected_track + 1) % self->ctx->tracks.len;
        }
        else if (IsKeyDown(KEY_UP))
        {

          if (self->ctx->tracks.len > MAX_MUSIC_SHOWING)
          {
            indexMove--;
            if (indexMove > self->ctx->tracks.len - MAX_MUSIC_SHOWING)
            {
              indexMove = 0;
            }
          }

          self->ctx->selected_track = (self->ctx->selected_track - 1 + self->ctx->tracks.len) % self->ctx->tracks.len;
        }
        time = 0;
        keyHeld = true;
      }
    }

    if (delay > maxDelay)
    {
      delay -= delayReduction;
    }
  }
  else
  {
    if (delay < minDelay)
    {
      delay += delayRecovery;
    }
    keyHeld = false;
  }

  if (IsKeyPressed(KEY_ENTER))
  {
    self->ctx->app_state = APP_PLAYING;
  }
}

bool SelectionMenu_IsShow(SelectionMenu *self)
{
  if (self->ctx->app_state == APP_SELECT)
  {
    self->isShow = true;
    return true;
  }

  self->isShow = false;
  return false;
}

void InitSelectionMenu(SelectionMenu *self, AppContext *ctx)
{
  self->ctx = ctx;

  self->rotationOffset = 0;
  self->targetRotationOffset = 0;

  self->selectionYOffset = 0;
  self->targetYOffset = 0;

  self->isShow = false;

  self->flying_objects = FlyingObject_Create(ctx);
}