#include <stdio.h>
#include "raylib.h"
#include "loading.h"
#include "context.h"
#include <stdlib.h>
#include "press_to_play.h"

int main()
{
  const int screenWidth = 600;
  const int screenHeight = 800;

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
  
  AppContext ctx = {
    .screen_height = screenHeight,
    .screen_width = screenWidth,
    .selected_track = 0,
    .app_state = APP_PRESS_TO_PLAY,
  };

  ctx.tracks.cap = 10;
  ctx.tracks.len = 1;
  ctx.tracks.track = malloc(sizeof(Track) * 10);
  

  Loading loading = {NULL};
  Drawable loading_draw = Loading_ToScene(&loading);
  
  PressToPlay press_to_play = {
    .ctx = &ctx
  };
  Drawable press_to_play_draw = PressToPlay_ToScene(&press_to_play);

  Drawable draws[] = {loading_draw, press_to_play_draw};
  
  int draws_len = 2;
  SetTargetFPS(60);
  InitAudioDevice();

  while (!WindowShouldClose())
  {
    for (int i = 0; i < draws_len; i++)
    {
      if (draws[i].scene->IsShow(draws[i].self))
      {
        draws[i].scene->Update(draws[i].self);
      }
    }

    BeginDrawing();

    ClearBackground(RAYWHITE);

    for (int i = 0; i < draws_len; i++)
    {
      if (draws[i].scene->IsShow(draws[i].self))
      {
        draws[i].scene->Draw(draws[i].self);
      }
    }

    EndDrawing();
  }
  CloseWindow();

  return 0;
}
