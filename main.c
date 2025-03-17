// #define TEST_CONTEXT
// Menghindari conflicting types: https://github.com/raysan5/raylib/issues/1217#issuecomment-893812805
#define WIN32_LEAN_AND_MEAN
#if defined(_WIN32)
#define WIN32
#endif
#if defined(_WIN64)
#define WIN64
#define _AMD64_
#undef _X86_
#else
#undef _AMD64_
#define _X86_
#endif
#include <minwindef.h>

#include <stdio.h>
#include "raylib.h"

// Library untuk raymath
// #define RAYMATH_IMPLEMENTATION
#include "raymath.h"

#include "background.h"
#include "loading.h"
#include "press_to_play.h"
#include "beatmap_creator.h"
#include "eotg.h"
#include "note.h"
#include "selection_menu.h"
#include "animasi.h"
#include "sfx.h"

#include "macro.h"

#include "context.h"
#include "gameplay.h"

#include "score.h"
#include <stdlib.h>


int _main()
{
  const int screenWidth = 600;
  const int screenHeight = 800;
  Image icon = LoadImage("resources/texture/game-icon.png");
  SetConfigFlags(FLAG_WINDOW_TOPMOST | FLAG_WINDOW_ALWAYS_RUN);
  
  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
  SetWindowIcon(icon);
  InitAudioDevice();

  AppContext ctx = CreateContext(screenWidth, screenHeight);
  // ctx.app_state = APP_BEATMAP_CREATOR;
  // ctx.app_state = APP_LOADING;
  ctx.app_state = APP_SELECT;
  Loading loading = {
    .ctx = &ctx
  };

    LoadingLoadTextures(&loading);
    LoadingInitPositions(&loading);

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

  SelectionMenu selection_menu;
  InitSelectionMenu(&selection_menu, &ctx);
  Drawable selection_menu_draw = SelectionMenu_ToScene(&selection_menu);
  
  Gameplay gameplay;
  InitGameplay(&gameplay,&ctx);
  Drawable gameplay_draw = Gameplay_ToScene(&gameplay);
  
  BeatmapCreator creator = CreateBeatmap(&ctx);
  Drawable creator_draw = BeatmapCreator_ToScene(&creator);

  ProgressBar bar;
  ScoreManager score_manager = InitScore(&ctx, &bar);
  Drawable score_draw = Score_ToScene(&score_manager);
  
  NoteManager note;
  InitNote(&note, &ctx, &gameplay, &score_manager);
  Drawable note_draw = Note_toScene(&note);

  // Drawable akan digambar dari urutan awal ke akhir. Untuk prioritas lebih tinggi, taruh Drawable di belakang
  Drawable draws[] = {loading_draw, press_to_play_draw, selection_menu_draw, creator_draw, gameplay_draw, score_draw, note_draw, bg_draw, eotg_draw};

  
  int draws_len = ARRAY_LEN(draws);
  SetTargetFPS(60);
  
  ctx.selected_track = 6;
  #ifdef TEST_CONTEXT 
    PlaySelectedTrack(&ctx);
  #endif
  
  while (!WindowShouldClose()) {
    UpdateContext(&ctx);

    if (ctx.app_state != APP_PLAYING) {
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_W) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_TRIGGER_1) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_TRIGGER_2)) {
          PlayArrowSfx(KEY_RIGHT);
        }
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_S) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_RIGHT_TRIGGER_1) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_RIGHT_TRIGGER_2)) {
          PlayArrowSfx(KEY_LEFT);
        }
        if (IsKeyPressed(KEY_ENTER)) {
            PlayEnterSfx();
        }
    }
    
    for (int i = 0; i < draws_len; i++)
    {
      if (draws[i].scene->IsShow(draws[i].self))
      {
        draws[i].scene->Update(draws[i].self);
      }
    }
    // printf("%d", note.note->isHit);

    BeginDrawing();

    ClearBackground(RAYWHITE);

    for (int i = 0; i < draws_len; i++)
    {
      if (draws[i].scene->IsShow(draws[i].self))
      {
        draws[i].scene->Draw(draws[i].self);
      }
    }

    // DrawLine(screenWidth*2/3, 0, screenWidth*2/3, screenHeight, BLACK);
    EndDrawing();
  }
  DestroyContext(&ctx);
  UnloadSfx();
  CloseAudioDevice();
  CloseWindow();
  UnloadImage(icon);

  return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* pCmdLine, int nCmdShow) {
  return _main();
}