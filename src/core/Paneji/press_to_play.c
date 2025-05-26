#include "press_to_play.h"
#include "context.h"
#include "sfx.h"

void PressToPlay_LoadTextures(PressToPlay *self) {
    self->logogame = LoadTexture("resources/texture/logo-game.png");

    const char *items[MENU_COUNT] = { "PILIH LAGU", "TUTORIAL", "EXIT" };
    Color colors[MENU_COUNT] = { RED, GREEN, BLUE };

    self->head = NULL;
    self->tail = NULL;
    self->menuterpilih = NULL;

    for (int i = 0; i < MENU_COUNT; i++) {
        MenuNode *node = malloc(sizeof(MenuNode));
        node->text = items[i];
        node->warna = colors[i];
        node->transisi = 1.0f;
        node->prev = self->tail;
        node->next = NULL;

        if (self->tail)
            self->tail->next = node;
        else
            self->head = node;

        self->tail = node;
    }

    self->menuterpilih = self->head;

    // Load sound dan BGM tetap sama
    self->enterSfx = LoadSound("resources/sfx/10.sfx-sementara.wav"); 
    self->menuBgm = LoadMusicStream("resources/backsound-mainmenu.mp3"); 
    self->MenuNaik = LoadSound("resources/sfx/7.sfx1.wav");     
    self->MenuTurun = LoadSound("resources/sfx/8.sfx2.wav"); 
    PlayMusicStream(self->menuBgm);
    self->isMusicPlaying = true;
}


void PressToPlay_UnloadTextures(PressToPlay *self){
    
    MenuNode *current = self->head;
    while (current) {
        MenuNode *next = current->next;
        free(current);
        current = next;
    }
    
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
    if (self->menuterpilih->next) {
        self->menuterpilih = self->menuterpilih->next;
        PlaySound(self->MenuNaik);
    }
    } else if (IsKeyPressed(KEY_F)) {
        if (self->menuterpilih->prev) {
            self->menuterpilih = self->menuterpilih->prev;
            PlaySound(self->MenuTurun);
        }
    } else if (IsKeyPressed(KEY_ENTER)) {
        PlaySound(self->enterSfx);
        StopMusicStream(self->menuBgm);
        self->isMusicPlaying = false;

    if (strcmp(self->menuterpilih->text, "PILIH LAGU") == 0) {
        self->ctx->app_state = APP_SELECT;
    } else if (strcmp(self->menuterpilih->text, "TUTORIAL") == 0) {
        self->ctx->app_state = APP_HOW_TO_PLAY;
    } else if (strcmp(self->menuterpilih->text, "EXIT") == 0) {
        CloseWindow();
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
    MenuNode *current = self->head;
    int i = 0;

    while (current) {
        Rectangle box = {
            .x = SCREEN_WIDTH / 2 - 150,
            .y = 350 + i * 150,
            .width = 300,
            .height = 75
        };

        Color bgColor = (current == self->menuterpilih) ? RAYWHITE : LIGHTGRAY;
        Color textColor = (current == self->menuterpilih) ? GOLD : GRAY;

        DrawRectangleRounded(box, 0.6f, 10, bgColor);
        Vector2 textSize = MeasureTextEx(self->ctx->font, current->text, 30, 2);
        Vector2 textPos = {
            box.x + (box.width - textSize.x) / 2,
            box.y + (box.height - textSize.y) / 2
        };

    DrawTextEx(self->ctx->font, current->text, textPos, 30, 2, textColor);

    current = current->next;
    i++;
}

}

bool PressToPlay_IsShow(PressToPlay *self){
    return self->ctx->app_state == APP_PRESS_TO_PLAY;
}

