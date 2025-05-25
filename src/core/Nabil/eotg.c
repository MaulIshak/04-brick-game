#include "eotg.h"
#include "context.h"
#include <stdio.h>
#include "raylib.h"
#include "macro.h"
#include "sfx.h"


void EndOfTheGame_Init(EndOfTheGame *self){
    Font gradeFont;
    gradeFont = LoadFontEx("resources/font/BungeeSpice-Regular.ttf", 175, NULL, 0);
    self->gradeFont = gradeFont;
    self->anim_accuracy = 0.0f;
    self->anim_point = 0;
    self->animation_timer = 0.0f;
    self->animation_done = false;
    self->count_played = false;
}

void EndOfTheGame_Draw(EndOfTheGame *self){
    (void)self;
    char buff[256] = {0};
    char* grade = "";
    Color gradeColor;
    
    // CREATE A CENTER RECTANGLE
    ClearBackground(WHITE);
    DrawCircleGradient(self->ctx->screen_width / 2, self->ctx->screen_height / 2, self->ctx->screen_height, (Color){254, 250, 148, 220}, (Color){255, 147, 98, 220});
    DrawCircleGradient(self->ctx->screen_width / 2 + 100, self->ctx->screen_height / 2 - 500, self->ctx->screen_height, (Color){255, 147, 98, 220}, (Color){255, 92, 93, 220});
    DrawCircleGradient(self->ctx->screen_width / 2 + 200, self->ctx->screen_height / 2 - 600, self->ctx->screen_height, (Color){255, 92, 93, 220}, (Color){128, 69, 255, 220});
    DrawCircleGradient(self->ctx->screen_width / 2 + 300, self->ctx->screen_height / 2 - 700, self->ctx->screen_height, (Color){128, 69, 255, 220}, (Color){57, 43, 214, 220});
    DrawCircleGradient(self->ctx->screen_width / 2 + 400, self->ctx->screen_height / 2 - 800, self->ctx->screen_height, (Color){57, 43, 214, 220}, (Color){24, 29, 149, 220});
    DrawCircle(self->ctx->screen_width / 2 + 500, self->ctx->screen_height / 2 - 900, self->ctx->screen_height, (Color){24, 29, 149, 220});

    // BeginBlendMode(BLEND_ALPHA);
    // DrawRectangle(0, 0, self->ctx->screen_width, self->ctx->screen_height, ColorAlpha(BLACK, 0.3));
    DrawRectangle(50, (self->ctx->screen_height/2) / 2 - 50, self->ctx->screen_width - 100, self->ctx->screen_height/2 + 100, RAYWHITE);
    
    // DRAW GRADE
    float accuracy = self->anim_accuracy;
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
    
    // DRAW ACCURACY PERCENTAGE
    sprintf(buff, "%ld", self->anim_point);
    float pointWidth = MeasureTextEx(self->gradeFont, buff, 70, 0).x;
    sprintf(buff, "%.2f%%", self->anim_accuracy);
    float accuracyWidth = MeasureTextEx(self->gradeFont, buff, 70, 0).x;

    float separatorWidth = MeasureTextEx(self->gradeFont, "|", 70, 0).x;
    float totalWidth = pointWidth + separatorWidth + accuracyWidth + 20; 
    
    sprintf(buff, "%ld", self->anim_point);
    DrawTextEx(self->gradeFont, buff, (Vector2){ self->ctx->screen_width / 2 - totalWidth / 2, 320 }, 70, 0, BLACK);
    DrawTextEx(self->gradeFont, "|", (Vector2){ self->ctx->screen_width / 2 - totalWidth / 2 + pointWidth + 10, 320 }, 70, 0, BLACK);
    sprintf(buff, "%.2f%%", self->anim_accuracy);
    DrawTextEx(self->gradeFont, buff, (Vector2){ self->ctx->screen_width / 2 - totalWidth / 2 + pointWidth + separatorWidth + 20, 320 }, 70, 0, BLACK);
    
    // DRAW ACCURACY NOTE
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
    if (!self->count_played && !self->animation_done) {
        PlayCountScoreSfx();
        self->count_played = true;
    }
    
    if (!self->animation_done) {
        self->animation_timer += GetFrameTime();
        float t = self->animation_timer / 2.5f;
        if (t >= 1.0f) {
            t = 1.0f;
            self->animation_done = true;
        }
        self->anim_point = (long)(self->ctx->score.point * t);
        self->anim_accuracy = self->ctx->score.accuracy * t;
    }
    
    if(IsKeyPressed(KEY_D)) {
        self->ctx->app_state = APP_PLAYING;
        self->ctx->score.accuracy = 0.00;
        
        self->anim_accuracy = 0.0f;
        self->anim_point = 0;
        self->animation_timer = 0.0f;
        self->animation_done = false;
        self->count_played = false;
    }
    if (IsKeyPressed(KEY_K)) {
        self->ctx->app_state = APP_SELECT;
        self->ctx->score.accuracy = 0.00;
        
        self->anim_accuracy = 0.0f;
        self->anim_point = 0;
        self->animation_timer = 0.0f;
        self->animation_done = false;
        self->count_played = false;
    }
}

bool EndOfTheGame_IsShow(EndOfTheGame *self){
    if(self->ctx->app_state == END_OF_THE_GAME) {
        return true;
    }
    return false;
}