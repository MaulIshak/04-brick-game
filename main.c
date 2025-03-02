// #define TEST_CONTEXT

#include <stdio.h>
#include "raylib.h"

#include "background.h"
#include "loading.h"
#include "press_to_play.h"
#include "beatmap_creator.h"
#include "eotg.h"

#include "macro.h"

#include "context.h"
#include "gameplay.h"
#include <stdlib.h>


int main()
{
  const int screenWidth = 600;
  const int screenHeight = 800;

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
  InitAudioDevice();

  AppContext ctx = CreateContext(screenWidth, screenHeight);
  // ctx.app_state = APP_BEATMAP_CREATOR;
  ctx.app_state = END_OF_THE_GAME;
  Loading loading = {
    .ctx = &ctx
  };
  Drawable loading_draw = Loading_ToScene(&loading);
  EndOfTheGame eotg = {
    .ctx = &ctx
  };
  Drawable eotg_draw = EndOfTheGame_ToScene(&eotg);
  
  PressToPlay press_to_play = {
    .ctx = &ctx
  };

  Drawable press_to_play_draw = PressToPlay_ToScene(&press_to_play);

  Background bg = CreateBackground(&ctx);
  Drawable bg_draw = Background_ToScene(&bg);

  
  Gameplay gameplay;
  InitGameplay(&gameplay,&ctx);
  Drawable gameplay_draw = Gameplay_ToScene(&gameplay);
  
  BeatmapCreator creator = CreateBeatmap(&ctx);
  Drawable creator_draw = BeatmapCreator_ToScene(&creator);
  
  // Drawable akan digambar dari urutan awal ke akhir. Untuk prioritas lebih tinggi, taruh Drawable di belakang
  Drawable draws[] = {loading_draw, press_to_play_draw, creator_draw, gameplay_draw, bg_draw, eotg_draw};

  
  int draws_len = ARRAY_LEN(draws);
  SetTargetFPS(60);
  
  ctx.selected_track = 2;
  #ifdef TEST_CONTEXT 
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
