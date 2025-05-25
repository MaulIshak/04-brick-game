#include "press_to_play.h"
#include "context.h"
#include "sfx.h"

void PressToPlay_LoadTextures(PressToPlay *self){
    self->logogame = LoadTexture("resources/texture/logo-game.png");
    self->selectedIndex = 0;
    self->menuItems[0] = "PILIH LAGU";
    self->menuItems[1] = "TUTORIAL";
    self->menuItems[2] = "EXIT";
    self->WarnaAwal[0] =  RED;
    self->WarnaAwal[1] =  GREEN;
    self->WarnaAwal[2] =  BLUE;

    self->enterSfx = LoadSound("resources/sfx/10.sfx-sementara.wav"); 
    self->menuBgm = LoadMusicStream("resources/backsound-mainmenu.mp3"); 
    self->MenuNaik = LoadSound("resources/sfx/7.sfx1.wav");     
    self->MenuTurun = LoadSound("resources/sfx/8.sfx2.wav"); 
    PlayMusicStream(self->menuBgm);
    self->isMusicPlaying = true;

    for (int i = 0; i < MENU_COUNT; i++) {
    self->TransisiMenu[i] = 1.0f;
    }
}

void PressToPlay_UnloadTextures(PressToPlay *self){
    UnloadTexture(self->logogame);
    UnloadSound(self->enterSfx);
    UnloadSound(self->MenuNaik);
    UnloadSound(self->MenuTurun);
    UnloadMusicStream(self->menuBgm);
}

void PressToPlay_Update(PressToPlay *self) {
    FlyingObject_Update(&self->flying_objects, self->ctx);

    if (!self->isMusicPlaying) {
        PlayMusicStream(self->menuBgm);
        self->isMusicPlaying = true;
    }

    if (self->isMusicPlaying) {
        UpdateMusicStream(self->menuBgm);
    }

    if (IsKeyPressed(KEY_J)) {
        self->selectedIndex = (self->selectedIndex + 1) % MENU_COUNT;
        PlaySound(self->MenuNaik);
    } else if (IsKeyPressed(KEY_F)) {
        self->selectedIndex = (self->selectedIndex - 1 + MENU_COUNT) % MENU_COUNT;
        PlaySound(self->MenuTurun);
    } else if (IsKeyPressed(KEY_ENTER)) {
        PlaySound(self->enterSfx);
        StopMusicStream(self->menuBgm);
        self->isMusicPlaying = false;

        switch (self->selectedIndex) {
            case 0:
                self->ctx->app_state = APP_SELECT;
                break;
            case 1:
                self->ctx->app_state = APP_HOW_TO_PLAY;
                break;
            case 2:
                CloseWindow();
                break;
        }
    }
    for (int i = 0; i < MENU_COUNT; i++) {
        if (i == self->selectedIndex) {
            self->TransisiMenu[i] += (1.1f - self->TransisiMenu[i]) * 0.1f; // Smooth grow
        } else {
            self->TransisiMenu[i] += (1.0f - self->TransisiMenu[i]) * 0.1f; // Smooth shrink
        }
    }

}

void PressToPlay_Draw(PressToPlay *self){
    Font font = self->ctx->font;

    DrawCircleGradient(self->ctx->screen_width / 2, self->ctx->screen_height / 2, self->ctx->screen_height, (Color){254, 250, 148, 220}, (Color){255, 147, 98, 220});
    DrawCircleGradient(self->ctx->screen_width / 2 - 100, self->ctx->screen_height / 2 + 500, self->ctx->screen_height, (Color){255, 147, 98, 220}, (Color){255, 92, 93, 220});
    DrawCircleGradient(self->ctx->screen_width / 2 - 200, self->ctx->screen_height / 2 + 600, self->ctx->screen_height, (Color){255, 92, 93, 220}, (Color){128, 69, 255, 220});
    DrawCircleGradient(self->ctx->screen_width / 2 - 300, self->ctx->screen_height / 2 + 700, self->ctx->screen_height, (Color){128, 69, 255, 220}, (Color){57, 43, 214, 220});
    DrawCircleGradient(self->ctx->screen_width / 2 - 400, self->ctx->screen_height / 2 + 800, self->ctx->screen_height, (Color){57, 43, 214, 220}, (Color){24, 29, 149, 220});
    DrawCircle(self->ctx->screen_width / 2 - 500, self->ctx->screen_height / 2 + 900, self->ctx->screen_height, (Color){24, 29, 149, 220});
    
    FlyingObject_Draw(&self->flying_objects);
    
    DrawTextureEx(self->logogame, (Vector2){SCREEN_WIDTH / 2 - (self->logogame.width * 0.25f), 80}, 0, 0.5f, WHITE);
    for (int i = 0; i < MENU_COUNT; i++) {
        Rectangle box = {
        .x = SCREEN_WIDTH / 2 - 150,
        .y = 350 + i * 150,
        .width = 300,
        .height = 75
    };

        Color bgColor = (i == self->selectedIndex) ? RAYWHITE : LIGHTGRAY;
        Color textColor = (i == self->selectedIndex) ? GOLD : GRAY;

        DrawRectangleRounded(box,0.6f, 10, bgColor);
        DrawTextEx(self->ctx->font, self->menuItems[i],
            (Vector2){ box.x + 20, box.y + 10 }, 30, 2, textColor);
    }
}

bool PressToPlay_IsShow(PressToPlay *self){
    return self->ctx->app_state == APP_PRESS_TO_PLAY;
}

