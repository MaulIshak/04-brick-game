#include <stdio.h>
#include "raylib.h"
#include "score.h"

ScoreManage InitScore(AppContext *ctx)
{
    ScoreManage score;

    score.value = 1000000;

    score.ctx = ctx;

    return score;
}

void AddScore(ScoreManage *score, int amount)
{
    score->value =+ amount;
}

void DrawScore(ScoreManage *score)
{
    char scoreText[20];
    sprintf(scoreText, "Score %d", score->value);
    DrawRectangle();
    DrawText(scoreText, score->ctx->screen_width - 170, 20, 24, BLACK);
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