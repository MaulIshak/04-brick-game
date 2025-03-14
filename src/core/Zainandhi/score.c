#include <stdio.h>
#include "raylib.h"
#include "score.h"
#include "gameplay.h"
#include "context.h"

ScoreManager InitScore(AppContext *ctx, Gameplay *gameplay)
{
    ScoreManager score;

    score.isBeatmapLoaded = false;

    score.value = 0;

    score.ctx = ctx;

    score.width = ctx->screen_width - gameplay->width;

    score.perfectCombo = 0;

    score.currentFrame = 0;

    score.frameTimer = 0;

    score.fireFrames[0] = LoadTexture("resources/texture/fire1.png");
    score.fireFrames[1] = LoadTexture("resources/texture/fire2.png");
    score.fireFrames[2] = LoadTexture("resources/texture/fire3.png");

    return score;
}

void AddScore(ScoreManager *score, Accuracy acc)
{
    int totalScore = 0;  
    int perfect = 100;
    int good = 50;
    int miss = 0;

    int sumPerfect = 0;
    int sumGood = 0;
    int sumMiss = 0;

    int totalNotesHit = 0; 

    totalNotesHit++;

    if (acc == PERFECT)
    {
        score->value += perfect;
        score->perfectCombo++;
        sumPerfect++;
    }
    else if (acc == GOOD)
    {
        score->value += good;
        score->perfectCombo = 0;
        sumGood++;
    }
    else
    {
        score->value += miss;
        score->perfectCombo = 0;
        sumMiss++;
    }

    if (totalNotesHit > 0)
    {
        score->ctx->score.accuracy = ((sumPerfect + (0.5 * sumGood)) / (float)totalNotesHit) * 100;
    }
}

void DrawScore(ScoreManager *score)
{
    char scoreText[20];
    sprintf(scoreText, "%d", score->value);
    DrawRectangle(score->ctx->screen_width - score->width, 0, score->width, score->ctx->screen_height, BLACK);
    
    // if (score->perfectCombo >= 1)
    // {
    //     score->frameTimer += GetFrameTime();
    //     if (score->frameTimer >= 0.2f)
    //     {
    //         score->currentFrame = (score->currentFrame + 1) % 3;
    //         score->frameTimer = 0;
    //     }

    //     int fireWidth = score->fireFrames[score->currentFrame].width;

    //     DrawTexture(score->fireFrames[score->currentFrame], (score->ctx->screen_width - score->width + (score->width / 2) - (fireWidth / 2)), 50, WHITE);
    // }
    
    DrawText("Score", score->ctx->screen_width - score->width + (score->width / 2) - (MeasureText("Score", 20) / 2), 130, 20, WHITE);
    DrawText(scoreText, score->ctx->screen_width - score->width + (score->width / 2) - (MeasureText(scoreText, 20) / 2), 160, 20, WHITE);

    static int scoreIncrease = 0;
    static float scoreTimer = 0;
    static int lastScore = 0;

    if (score->value > lastScore)
    {
        scoreIncrease = score->value - lastScore;
        scoreTimer = 1.5f;
    }

    lastScore = score->value;

    if (scoreIncrease > 0 && scoreTimer > 0)
    {
        char scoreIncreaseText[10];
        sprintf(scoreIncreaseText, "+%d", scoreIncrease);

        int alpha = (int)(255 * (scoreTimer / 1.5f));

        DrawText(scoreIncreaseText, score->ctx->screen_width - score->width + (score->width / 2) - (MeasureText(scoreIncreaseText, 20) / 2), 70, 20, Fade(WHITE, alpha / 255.0f));

        scoreTimer -= GetFrameTime();
        if (scoreTimer <= 0)
        {
            scoreIncrease = 0;
        }
    }
}

void UpdateScore(ScoreManager *score)
{
    if (!score->isBeatmapLoaded)
    {
        score->beatmap = GetSelectedMusicBeatmap(score->ctx);
        score->isBeatmapLoaded = true;
    }

    score->ctx->score.point = score->value;
}

bool IsShowScore(ScoreManager *score)
{
    if (score->ctx->app_state == APP_PLAYING)
    {
        return true;
    }
    return false;
}

void LoadingUnloadTexturesFire(ScoreManager *score)
{
    for (int i = 0; i < 3; i++)
    {
        if (score->fireFrames[i].id != 0)
        {
            UnloadTexture(score->fireFrames[i]);
        }
    }
}