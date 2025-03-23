#include "eotg.h"
#include "context.h"
#include <stdio.h>
#include "raylib.h"
#include "macro.h"


void EndOfTheGame_Init(EndOfTheGame *self){
    Font gradeFont;
    gradeFont = LoadFontEx("resources/font/BungeeSpice-Regular.ttf", 175, NULL, 0);
    self->gradeFont = gradeFont;
}

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
        gradeColor = GREEN;
    } else if (accuracy >= 70) {
        grade = "B";
        gradeColor = BLUE;
    } else if (accuracy >= 60) {
        grade = "C";
        gradeColor = ORANGE;
    } else {
        grade = "D";
        gradeColor = RED;
    }
    
    DrawTextEx(self->gradeFont, grade, (Vector2){ self->ctx->screen_width / 2 - MeasureTextEx(self->gradeFont, grade, 174, 0).x / 2, 160 }, 174, 0, gradeColor);
    sprintf(buff, "%ld", self->ctx->score.point);
    DrawTextEx(self->gradeFont, buff, (Vector2){ self->ctx->screen_width / 2 - MeasureTextEx(self->gradeFont, buff, 90, 0).x / 2, 320 }, 90, 0, BLACK);
    sprintf(buff, "%ld", self->ctx->score.accuracy);
    DrawTextEx(self->gradeFont, buff, (Vector2){ self->ctx->screen_width / 2 - MeasureTextEx(self->gradeFont, buff, 90, 0).x / 2, 320 }, 90, 0, BLACK);
    
    // DRAW ACCURACY
    DrawRectangle(50, (self->ctx->screen_height/2) / 2 + 220, self->ctx->screen_width - 100, self->ctx->screen_height/8 - 5, DARKGRAY);
    sprintf(buff, "%d", self->ctx->score.perfect);
    DrawTextEx(self->gradeFont, "Perfect", (Vector2){ self->ctx->screen_width / 5, 465 }, 32, 0, YELLOW);
    DrawTextEx(self->gradeFont, buff, (Vector2){ self->ctx->screen_width / 5 + MeasureTextEx(self->gradeFont, "Perfect", 32, 0).x / 2 - MeasureTextEx(self->gradeFont, buff, 32, 0).x / 2, 435 }, 32, 0, RAYWHITE);
    
    sprintf(buff, "%d", self->ctx->score.good);
    DrawTextEx(self->gradeFont, "Good", (Vector2){ self->ctx->screen_width / 2 - MeasureTextEx(self->gradeFont, "Good", 32, 0).x / 2, 465 }, 32, 0, GREEN);
    DrawTextEx(self->gradeFont, buff, (Vector2){ self->ctx->screen_width / 2 - MeasureTextEx(self->gradeFont, buff, 32, 0).x / 2, 435 }, 32, 0, RAYWHITE);
    
    sprintf(buff, "%d", self->ctx->score.miss);
    DrawTextEx(self->gradeFont, "Miss", (Vector2){ self->ctx->screen_width * 4.5 / 6.3 - MeasureTextEx(self->gradeFont, "Miss", 32, 0).x / 2, 465 }, 32, 0, (Color){229, 25, 45, 255});
    DrawTextEx(self->gradeFont, buff, (Vector2){ self->ctx->screen_width * 4.5 / 6.3 - MeasureTextEx(self->gradeFont, buff, 32, 0).x / 2, 435 }, 32, 0, RAYWHITE);
    
    // DRAW BUTTON Restart
    DrawRectangle((self->ctx->screen_width/4) / 2, (self->ctx->screen_height/2) + 150, 200, 60, PRIMARY_COLOR);
    DrawTextEx(self->gradeFont, "Restart [D]", (Vector2){ (self->ctx->screen_width/4) / 2 + 100 - MeasureTextEx(self->gradeFont, "Restart [D]", 28, 0).x / 2, (self->ctx->screen_height/2) + 165 }, 28, 0, WHITE);

    // DRAW BUTTON Continue
    DrawRectangle(((self->ctx->screen_width/4) / 2 + 6) * 4, (self->ctx->screen_height/2) + 150, 200, 60, SECONDARY_COLOR);
    DrawTextEx(self->gradeFont, "Continue [K]", (Vector2){ ((self->ctx->screen_width/4) / 2 + 6) * 4 + 100 - MeasureTextEx(self->gradeFont, "Continue [K]", 28, 0).x / 2, (self->ctx->screen_height/2) + 165 }, 28, 0, WHITE);
    
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