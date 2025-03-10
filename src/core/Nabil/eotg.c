#include "eotg.h"
#include "context.h"
#include <stdio.h>
#include "raylib.h"
#include "macro.h"

void EndOfTheGame_Draw(EndOfTheGame *self){
    (void)self;
    char buff[256] = {0};
    char* grade = "PERFECT";
    // CREATE A CENTER RECTANGLE
    BeginBlendMode(BLEND_ALPHA);
    DrawRectangle(0, 0, self->ctx->screen_width, self->ctx->screen_height, ColorAlpha(BLACK, 0.3));
    DrawRectangle(50, (self->ctx->screen_height/2) / 2 - 50, self->ctx->screen_width - 100, self->ctx->screen_height/2 + 100, RAYWHITE);

    // DRAW GRADE
    DrawText(grade, self->ctx->screen_width/2 - MeasureText(grade, 52) / 2, 200, 52, BLACK);
    sprintf(buff, "%ld", 13880); // Sign with self->ctx->score.point
    DrawText(buff, self->ctx->screen_width/2 - MeasureText(buff, 110) / 2, 270, 110, BLACK);

    // DRAW ACCURACY
    sprintf(buff, "%d", 120); // Sign with self->ctx->score.perfect
    DrawText("Perfect", self->ctx->screen_width/6 - 10, 465, 32, BLACK);
    DrawText(buff, self->ctx->screen_width/6 - 10 + MeasureText("Perfect", 32) / 2 - MeasureText(buff, 32) / 2, 435, 32, BLACK);
    sprintf(buff, "%d", 34); // Sign with self->ctx->score.good
    DrawText("Good", self->ctx->screen_width/2 - MeasureText("Good", 32) / 2, 465, 32, BLACK);
    DrawText(buff, self->ctx->screen_width/2 - MeasureText(buff, 32) / 2, 435, 32, BLACK);
    sprintf(buff, "%d", 12); // Sign with self->ctx->score.miss
    DrawText("Miss", self->ctx->screen_width * 4.5 / 6 - MeasureText("Miss", 32) / 2, 465, 32, BLACK);
    DrawText(buff, self->ctx->screen_width * 4.5 / 6 - MeasureText(buff, 32) / 2, 435, 32, BLACK);

    // DRAW BUTTON Restart
    DrawRectangle((self->ctx->screen_width/4) / 2, (self->ctx->screen_height/2) + 150,200, 60, PRIMARY_COLOR);
    DrawText("<- Restart", (self->ctx->screen_width/4) / 2 + 100 - (MeasureText("<- Restart", 32) / 2), (self->ctx->screen_height/2) + 165, 32, WHITE);
    // DRAW BUTTON Continue
    DrawRectangle(((self->ctx->screen_width/4) / 2 + 6) * 4, (self->ctx->screen_height/2) + 150,200, 60, SECONDARY_COLOR);
    DrawText("Continue ->", ((self->ctx->screen_width/4) / 2 + 6) * 4 + 100 - (MeasureText("Continue ->", 32) / 2), (self->ctx->screen_height/2) + 165, 32, WHITE);

    EndBlendMode();
}
void EndOfTheGame_Update(EndOfTheGame *self){
    if(IsKeyPressed(KEY_LEFT)) {
        self->ctx->app_state = APP_SELECT;
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        self->ctx->app_state = APP_PRESS_TO_PLAY;
    }
}
bool EndOfTheGame_IsShow(EndOfTheGame *self){
    if(self->ctx->app_state == END_OF_THE_GAME) {
        return true;
    }
    return false;
}