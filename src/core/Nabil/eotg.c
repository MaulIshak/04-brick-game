#include "eotg.h"
#include "context.h"
#include <stdio.h>
#include "raylib.h"
#include "macro.h"

void EndOfTheGame_Draw(EndOfTheGame *self){
    (void)self;
    char buff[256] = {0};
    char* grade = "";
    // CREATE A CENTER RECTANGLE
    BeginBlendMode(BLEND_ALPHA);
    DrawRectangle(0, 0, self->ctx->screen_width, self->ctx->screen_height, ColorAlpha(BLACK, 0.3));
    DrawRectangle(50, (self->ctx->screen_height/2) / 2 - 50, self->ctx->screen_width - 100, self->ctx->screen_height/2 + 100, RAYWHITE);

    // DRAW GRADE
    
    float accuracy = ((float)self->ctx->score.point / (float)((self->ctx->score.perfect + self->ctx->score.good + self->ctx->score.miss) * 100)) * 100;

    if (accuracy >= 90) {
        grade = "S";
    } else if (accuracy >= 80) {
        grade = "A";
    } else if (accuracy >= 70) {
        grade = "B";
    } else if (accuracy >= 60) {
        grade = "C";
    } else {
        grade = "D";
    }
    DrawText(grade, self->ctx->screen_width/2 - MeasureText(grade, 110) / 2, 180, 110, BLACK);
    sprintf(buff, "%ld", self->ctx->score.point);
    DrawText(buff, self->ctx->screen_width/2 - MeasureText(buff, 90) / 2, 320, 90, BLACK);

    // DRAW ACCURACY
    sprintf(buff, "%d", self->ctx->score.perfect); 
    DrawText("Perfect", self->ctx->screen_width/6 - 10, 465, 32, BLACK);
    DrawText(buff, self->ctx->screen_width/6 - 10 + MeasureText("Perfect", 32) / 2 - MeasureText(buff, 32) / 2, 435, 32, BLACK);
    sprintf(buff, "%d", self->ctx->score.good); 
    DrawText("Good", self->ctx->screen_width/2 - MeasureText("Good", 32) / 2, 465, 32, BLACK);
    DrawText(buff, self->ctx->screen_width/2 - MeasureText(buff, 32) / 2, 435, 32, BLACK);
    sprintf(buff, "%d", self->ctx->score.miss); 
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
        self->ctx->app_state = APP_PLAYING;
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        self->ctx->app_state = APP_SELECT;
    }
}
bool EndOfTheGame_IsShow(EndOfTheGame *self){
    if(self->ctx->app_state == END_OF_THE_GAME) {
        return true;
    }
    return false;
}