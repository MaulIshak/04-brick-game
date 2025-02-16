#include <stdio.h>
#include "raylib.h"
#include "loading.h"
#include "context.h"
#include <stdlib.h>
#include "press_to_play.h"

// #define TEST_CONTEXT

int main()
{
  const int screenWidth = 600;
  const int screenHeight = 800;

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
  InitAudioDevice();

  AppContext ctx = CreateContext(screenWidth, screenHeight);

  

  Loading loading = {
    .ctx = &ctx
  };
  Drawable loading_draw = Loading_ToScene(&loading);
  
  PressToPlay press_to_play = {
    .ctx = &ctx
  };
  Drawable press_to_play_draw = PressToPlay_ToScene(&press_to_play);

  Drawable draws[] = {loading_draw, press_to_play_draw};
  
  int draws_len = 2;
  SetTargetFPS(60);
  #ifdef TEST_CONTEXT 
    ctx.selected_track = 1;
    PlaySelectedTrack(&ctx);
  #endif
  while (!WindowShouldClose())
  {
    UpdateContext(&ctx);
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
  DestroyContext(&ctx);
  CloseAudioDevice();
  CloseWindow();

  return 0;
}
