#include <stdio.h>
#include "raylib.h"
#include "score.h"
#include "gameplay.h"

ScoreManage InitScore(AppContext *ctx, Gameplay *gameplay)
{
    ScoreManage score;

    score.value = 1000000;

    score.ctx = ctx;

    score.width = ctx->screen_width - gameplay->width; 

    return score;
}

// void AddScore(ScoreManage *score, int amount)
// {
//     score->value =+ amount;
// }

void DrawScore(ScoreManage *score)
{
    char scoreText[20];
    sprintf(scoreText, "Score %d", score->value);
    DrawRectangle(score->ctx->screen_width - score->width, 0, score->width, score->ctx->screen_height, DARKGRAY);
    DrawText(scoreText, score->ctx->screen_width - score->width + (score->width/2) - (MeasureText(scoreText, 20)/2), 100, 20, WHITE);
}

void UpdateScore(ScoreManage *score)
{

}

bool IsShowScore(ScoreManage *score)
{
    if(score->ctx->app_state == APP_PLAYING){
        return true;
    }
    return false;
}