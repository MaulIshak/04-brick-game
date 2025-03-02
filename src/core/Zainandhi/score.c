#include <stdio.h>
#include "raylib.h"
#include "score.h"
#include "gameplay.h"
#include "context.h"

ScoreManager InitScore(AppContext *ctx, Gameplay *gameplay)
{
    // AppContext ctx;

    ScoreManager score;

    score.isBeatmapLoaded = false;

    score.value = 0;

    score.ctx = ctx;

    score.width = ctx->screen_width - gameplay->width;

    return score;
}

void AddScore(ScoreManager *score, Accuracy acc)
{
    int maxScore = 1000; 

    int perfect = 100;
    int good = 50;
    int miss = 0;

    if (acc == PERFECT)
    {
        score->value = score->value + perfect;
    }
    else if (acc == GOOD)
    {
        score->value = score->value + good;
    }
    else
    {
        score->value = score->value + miss;
    }
}

void DrawScore(ScoreManager *score)
{
    char scoreText[20];
    sprintf(scoreText, "Score %d", score->value);
    DrawRectangle(score->ctx->screen_width - score->width, 0, score->width, score->ctx->screen_height,BLACK);
    DrawText(scoreText, score->ctx->screen_width - score->width + (score->width / 2) - (MeasureText(scoreText, 20) / 2), 100, 20, WHITE);
}

void UpdateScore(ScoreManager *score)
{
    if (!score->isBeatmapLoaded)
    {
        score->beatmap = GetSelectedMusicBeatmap(score->ctx);
        score->isBeatmapLoaded = true;
    }
}

bool IsShowScore(ScoreManager *score)
{
    if (score->ctx->app_state == APP_PLAYING)
    {
        return true;
    }
    return false;
}