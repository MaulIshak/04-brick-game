#include "eotg.h"
#include "context.h"
#include <stdio.h>
#include "raylib.h"
#include "macro.h"

void EndOfTheGame_Draw(EndOfTheGame *self){
    (void)self;
    char buff[256] = {0};
    char* grade = "";
    Color gradeColor;
    
    // CREATE A CENTER RECTANGLE
    BeginBlendMode(BLEND_ALPHA);
    DrawRectangle(0, 0, self->ctx->screen_width, self->ctx->screen_height, ColorAlpha(BLACK, 0.3));
    DrawRectangle(50, (self->ctx->screen_height/2) / 2 - 50, self->ctx->screen_width - 100, self->ctx->screen_height/2 + 100, RAYWHITE);
    
    // DRAW GRADE
    float accuracy = ((float)self->ctx->score.point / (float)((self->ctx->score.perfect + self->ctx->score.good + self->ctx->score.miss) * 100)) * 100;
    if (accuracy >= 90) {
        grade = "S";
        gradeColor = GOLD;
    } else if (accuracy >= 80) {
        grade = "A";
        gradeColor = DARKGREEN;
    } else if (accuracy >= 70) {
        grade = "B";
        gradeColor = DARKBLUE;
    } else if (accuracy >= 60) {
        grade = "C";
        gradeColor = ORANGE;
    } else {
        grade = "D";
        gradeColor = RED;
    }
    DrawTextEx(self->ctx->font, grade, (Vector2){ self->ctx->screen_width / 2 - MeasureTextEx(self->ctx->font, grade, 175, 0).x / 2, 160 }, 175, 0, gradeColor);
    sprintf(buff, "%ld", self->ctx->score.point);
    DrawTextEx(self->ctx->font, buff, (Vector2){ self->ctx->screen_width / 2 - MeasureTextEx(self->ctx->font, buff, 90, 0).x / 2, 320 }, 90, 0, BLACK);
    
    // DRAW ACCURACY
    DrawRectangle(50, (self->ctx->screen_height/2) / 2 + 220, self->ctx->screen_width - 100, self->ctx->screen_height/8 - 5, DARKGRAY);
    sprintf(buff, "%d", self->ctx->score.perfect);
    DrawTextEx(self->ctx->font, "Perfect", (Vector2){ self->ctx->screen_width / 5, 465 }, 32, 0, RAYWHITE);
    DrawTextEx(self->ctx->font, buff, (Vector2){ self->ctx->screen_width / 5 + MeasureTextEx(self->ctx->font, "Perfect", 32, 0).x / 2 - MeasureTextEx(self->ctx->font, buff, 32, 0).x / 2, 435 }, 32, 0, RAYWHITE);
    
    sprintf(buff, "%d", self->ctx->score.good);
    DrawTextEx(self->ctx->font, "Good", (Vector2){ self->ctx->screen_width / 2 - MeasureTextEx(self->ctx->font, "Good", 32, 0).x / 2, 465 }, 32, 0, RAYWHITE);
    DrawTextEx(self->ctx->font, buff, (Vector2){ self->ctx->screen_width / 2 - MeasureTextEx(self->ctx->font, buff, 32, 0).x / 2, 435 }, 32, 0, RAYWHITE);
    
    sprintf(buff, "%d", self->ctx->score.miss);
    DrawTextEx(self->ctx->font, "Miss", (Vector2){ self->ctx->screen_width * 4.5 / 6.3 - MeasureTextEx(self->ctx->font, "Miss", 32, 0).x / 2, 465 }, 32, 0, RAYWHITE);
    DrawTextEx(self->ctx->font, buff, (Vector2){ self->ctx->screen_width * 4.5 / 6.3 - MeasureTextEx(self->ctx->font, buff, 32, 0).x / 2, 435 }, 32, 0, RAYWHITE);
    
    // DRAW BUTTON Restart
    DrawRectangle((self->ctx->screen_width/4) / 2, (self->ctx->screen_height/2) + 150, 200, 60, PRIMARY_COLOR);
    DrawTextEx(self->ctx->font, "<- Restart", (Vector2){ (self->ctx->screen_width/4) / 2 + 100 - MeasureTextEx(self->ctx->font, "<- Restart", 32, 0).x / 2, (self->ctx->screen_height/2) + 165 }, 32, 0, WHITE);

    // DRAW BUTTON Continue
    DrawRectangle(((self->ctx->screen_width/4) / 2 + 6) * 4, (self->ctx->screen_height/2) + 150, 200, 60, SECONDARY_COLOR);
    DrawTextEx(self->ctx->font, "Continue ->", (Vector2){ ((self->ctx->screen_width/4) / 2 + 6) * 4 + 100 - MeasureTextEx(self->ctx->font, "Continue ->", 32, 0).x / 2, (self->ctx->screen_height/2) + 165 }, 32, 0, WHITE);
    
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