#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "accuracy.h"
#include "gameplay.h"
#include "context.h"

ScoreManager InitAcc(AppContext *ctx, Gameplay *gameplay){
    ScoreManager score;

    score.isBeatmapLoaded = false;
    
    score.ctx = ctx;

    score.ctx->score.accuracy = 0.00;

    return score;
}

void AddAcc(ScoreManager *score, Accuracy acc){
    int sumPerfect = 0;
    int sumGood = 0;
    int sumMiss = 0;

    if (acc == PERFECT)
    {
        sumPerfect++;
    }
    else if (acc == GOOD)
    {
        sumGood++;
    }
    else
    {
        sumMiss++;
    }

    score->ctx->score.accuracy += ((((sumPerfect * 1) + (sumGood * 0.5)) / score->beatmap.len) * (100));
    
}

void DrawAcc(ScoreManager *score){
    char accuracyText[20];

    Vector2 triangle[3] = {
        {510, score->ctx->screen_width + 120},
        {480, score->ctx->screen_width + 150},
        {510, score->ctx->screen_width + 150}
    };

    DrawTriangle(triangle[0], triangle[1], triangle[2], BLACK);

    DrawRectangle(score->ctx->screen_width - score->width + 60, score->ctx->screen_height - 80, score->width - 60, score->height - 20, BLACK);

    sprintf(accuracyText, "%.2f%%", score->ctx->score.accuracy);

    DrawTextEx(score->ctx->font, accuracyText, (Vector2){score->ctx->screen_width - score->width + 30 + (score->width / 2) - (MeasureText(accuracyText, 29) / 2), score->ctx->screen_height - 80}, 29, 1, WHITE);
}

void UpdateAcc(ScoreManager *score){
    if (IsSelectedMusicEnd(score->ctx))
    {
        score->ctx->score.accuracy = 0.00;
    }
}

bool IsShowAcc(ScoreManager *score){
    if (score->ctx->app_state == APP_PLAYING)
    {
        return true;
    }
    return false;
}