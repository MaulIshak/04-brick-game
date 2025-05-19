#include "press_to_play.h"
#include "context.h"
    #include "sfx.h"

void PressToPlay_LoadTextures(PressToPlay *self){
    self->logogame = LoadTexture("resources/texture/logo-game.png");
    self->selectedIndex = 0;
    self->menuItems[0] = "MENU 1";
    self->menuItems[1] = "MENU 2";
    self->menuItems[2] = "MENU 3";
}

void PressToPlay_UnloadTextures(PressToPlay *self){
    UnloadTexture(self->logogame);
}

void PressToPlay_Update(PressToPlay *self) {
    if (IsKeyPressed(KEY_DOWN)) {
        self->selectedIndex = (self->selectedIndex + 1) % MENU_COUNT;
    } else if (IsKeyPressed(KEY_UP)) {
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

    DrawTextureEx(self->logogame, (Vector2){SCREEN_WIDTH / 2 - (self->logogame.width * 0.25f), 80}, 0, 0.5f, WHITE);
    DrawTexture()
    for (int i = 0; i < MENU_COUNT; i++) {
        Rectangle box = {
        .x = SCREEN_WIDTH / 2 - 150,
        .y = 400 + i * 100,
        .width = 300,
        .height = 70
    };

        Color bgColor = (i == self->selectedIndex) ? DARKBLUE : GRAY;
        Color textColor = (i == self->selectedIndex) ? YELLOW : WHITE;

        DrawRectangleRounded(box, 0.2f, 10, bgColor);
        DrawTextEx(self->ctx->font, self->menuItems[i],
            (Vector2){ box.x + 20, box.y + 10 }, 30, 2, textColor);
    }
}

bool PressToPlay_IsShow(PressToPlay *self){
    return self->ctx->app_state == APP_PRESS_TO_PLAY;
}
