#include "press_to_play.h"
#include "context.h"
#include "sfx.h"

void PressToPlay_LoadTextures(PressToPlay *self){
    self->logogame = LoadTexture("resources/texture/logo-game.png");
    self->selectedIndex = 0;
    self->menuItems[0] = "MENU 1";
    self->menuItems[1] = "MENU 2";
    self->menuItems[2] = "MENU 3";
    self->WarnaAwal[0] =  RED;
    self->WarnaAwal[1] =  GREEN;
    self->WarnaAwal[2] =  BLUE;
}

void PressToPlay_UnloadTextures(PressToPlay *self){
    UnloadTexture(self->logogame);
}

void PressToPlay_Update(PressToPlay *self) {
    if (IsKeyPressed(KEY_F)) {
        self->selectedIndex = (self->selectedIndex + 1) % MENU_COUNT;
    } else if (IsKeyPressed(KEY_J)) {
        self->selectedIndex = (self->selectedIndex - 1 + MENU_COUNT) % MENU_COUNT;
    } else if (IsKeyPressed(KEY_ENTER)) {
        switch (self->selectedIndex) {
            case 0:
                self->ctx->app_state = APP_SELECT;
                break;
            case 1:
                break;
            case 2:
                CloseWindow();
                break;
        }
    }
}

void PressToPlay_Draw(PressToPlay *self){
    Font font = self->ctx->font;

    DrawTextureEx(self->logogame, (Vector2){SCREEN_WIDTH / 2 - (self->logogame.width * 0.25f), 80}, 0, 0.5f, WHITE);
    for (int i = 0; i < MENU_COUNT; i++) {
        Rectangle box = {
        .x = SCREEN_WIDTH / 2 - 150,
        .y = 350 + i * 150,
        .width = 300,
        .height = 100
    };

        Color bgColor = (i == self->selectedIndex) ? DARKBLUE : BLUE;
        Color textColor = (i == self->selectedIndex) ? YELLOW : WHITE;

        DrawRectangleRounded(box,0.6f, 10, bgColor);
        DrawTextEx(self->ctx->font, self->menuItems[i],
            (Vector2){ box.x + 20, box.y + 10 }, 30, 2, textColor);
    }
}

bool PressToPlay_IsShow(PressToPlay *self){
    return self->ctx->app_state == APP_PRESS_TO_PLAY;
}
