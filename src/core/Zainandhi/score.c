#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "score.h"
#include "gameplay.h"
#include "context.h"
#include "sfx.h"
#include "progress_bar.h"

ScoreManager InitScore(AppContext *ctx, Gameplay *gameplay)
{
    ScoreManager score = {0};

    score.isBeatmapLoaded = false;

    score.value = 0;

    score.comboValue = 0;

    score.perfect = 0;

    score.good = 0;

    score.miss = 0;

    score.ctx = ctx;

    score.width = gameplay->padPositions->x + 75;

    score.height = 50;

    score.perfectCombo = 0;

    score.currentFrame = 0;

    score.frameTimer = 0;

    return score;
}

void LoadingLoadTexturesFire(ScoreManager *score)
{
    // fireFrames[0] = LoadTexture("resources/texture/fire1.png");
    // fireFrames[1] = LoadTexture("resources/texture/fire2.png");
    // fireFrames[2] = LoadTexture("resources/texture/fire3.png");
}

void AddScore(ScoreManager *score, Accuracy acc)
{
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
        PlayPerfectSfx();
        score->ctx->score.point += perfect;
        score->perfectCombo++;
        sumPerfect++;
        score->perfect++;
        score->comboValue++;
        score->comboShow = true;
    }
    else if (acc == GOOD)
    {
        PlayGoodSfx();
        score->ctx->score.point += good;
        score->perfectCombo = 0;
        sumGood++;
        score->good++;
        score->comboValue++;
        score->comboShow = true;
    }
    else
    {
        PlayMissSfx();
        score->ctx->score.point += miss;
        score->perfectCombo = 0;
        sumMiss++;
        score->miss++;
        score->comboValue = 0;
        score->comboShow = false;
    }

    AddAcc(score, acc);
}

void DrawScore(ScoreManager *score)
{
    ProgressBar bar = {0};
    char scoreText[20], accuracyText[20], comboText[20];
    static int scoreIncrease = 0;
    static float scoreTimer = 0;
    static int lastScore = 0;
    static float comboScale = 1.0f;
    static float comboTimer = 0.0f;
    int alpha = (int)(255 * (scoreTimer / 1.5f));
    sprintf(scoreText, "%d",  score->ctx->score.point);
    sprintf(accuracyText, "%.2f%%", score->ctx->score.accuracy);
    sprintf(comboText, "Combo!!\n %d", score->comboValue);

    Vector2 bottomTriangle[3] = {
        {450, score->ctx->screen_width + 150},
        {400, score->ctx->screen_width + 200},
        {450, score->ctx->screen_width + 200}};

    DrawTriangle(bottomTriangle[0], bottomTriangle[1], bottomTriangle[2], WHITE);

    DrawRectangle(score->ctx->screen_width - score->width, score->ctx->screen_height - 50, score->width, score->height, WHITE);

    DrawRectangle(score->ctx->screen_width - score->width + 60, score->ctx->screen_height - 80, score->width - 60, score->height - 20, BLACK);
    if (score->comboShow)
    {
        comboTimer += GetFrameTime() * 10;
        comboScale = 1.0f + 0.1f * sin(comboTimer);

        DrawTextPro(
            score->ctx->font,
            comboText,
            (Vector2){460, 300},
            (Vector2){0, 0},
            -15.0,
            40 * comboScale,
            1,
            Fade(BLACK, alpha / 255.0f));
    }

    DrawTextEx(score->ctx->font, scoreText, (Vector2){score->ctx->screen_width - score->width + (score->width / 2) - (MeasureText(scoreText, 50) / 2), score->ctx->screen_height - 50}, 50, 1, BLACK);

    DrawTextEx(score->ctx->font, accuracyText, (Vector2){score->ctx->screen_width - score->width + 30 + (score->width / 2) - (MeasureText(accuracyText, 29) / 2), score->ctx->screen_height - 80}, 29, 1, WHITE);

    if (score->value > lastScore)
    {
        scoreIncrease = score->value - lastScore;
        scoreTimer = 1.5f;
    }

    lastScore = score->value;

    if (scoreIncrease > 0 && scoreTimer > 0)
    {
        char scoreIncreaseText[20];
        sprintf(scoreIncreaseText, "+%d Score!!", scoreIncrease);

        DrawText(scoreIncreaseText, score->ctx->screen_width - score->width + (score->width / 2) - (MeasureText(scoreIncreaseText, 20) / 2), score->ctx->screen_height - 120, 22, Fade(WHITE, alpha / 255.0f));

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

    score->ctx->score.perfect = score->perfect;
    score->ctx->score.good = score->good;
    score->ctx->score.miss = score->miss;
    if (IsSelectedMusicEnd(score->ctx) && (score->value > 0 || score->miss > 0))
    {
        if (score->value > GetSelectedTrack(score->ctx).high_score)
        {
            SetScoreAndAccuracy(score->ctx, score->value, score->ctx->score.accuracy);
        }
        score->value = 0;
        score->isBeatmapLoaded = false;
        score->perfect = 0;
        score->good = 0;
        score->miss = 0;
        score->perfectCombo = 0;

        score->ctx->score.perfect = 0;
        score->ctx->score.good = 0;
        score->ctx->score.miss = 0;
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