#include <stdio.h>
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

    // score.width = ctx->screen_width - bar->width;
    score.width = gameplay->padPositions->x + 5;
    // score.width = 70;

    score.height = 20;

    score.perfectCombo = 0;

    score.currentFrame = 0;

    score.frameTimer = 0;

    score.ctx->score.accuracy = 0.00;

    score.fireFrames[0] = LoadTexture("resources/texture/fire1.png");
    score.fireFrames[1] = LoadTexture("resources/texture/fire2.png");
    score.fireFrames[2] = LoadTexture("resources/texture/fire3.png");

    return score;
}

void AddScore(ScoreManager *score, Accuracy acc)
{
    // int totalScore = 0;
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
        score->value += perfect;
        score->perfectCombo++;
        sumPerfect++;
        score->perfect++;
        score->comboValue += perfect;
        score->comboShow = true;
    }
    else if (acc == GOOD)
    {
        PlayGoodSfx();
        score->value += good;
        score->perfectCombo = 0;
        sumGood++;
        score->good++;
        score->comboValue += perfect;
        score->comboShow = true;
    }
    else
    {
        PlayMissSfx();
        score->value += miss;
        score->perfectCombo = 0;
        sumMiss++;
        score->miss++;
        score->comboValue = 0;
        score->comboShow = false;
    }

    // if (totalNotesHit > 0)
    // {
    //     score->ctx->score.accuracy = ((sumPerfect + (0.5 * sumGood)) / (float)totalNotesHit) * 100;
    // }

    // if (totalNotesHit > 0)
    // {
    //     float newAccuracy = ((sumPerfect + (0.5 * sumGood)) / (float)totalNotesHit) * 100/100;

    //     score->ctx->score.accuracy = newAccuracy;
    // }

    // score->beatmap.len

    score->ctx->score.accuracy += ((((sumPerfect * 1) + (sumGood * 0.5)) / score->beatmap.len) * (100));
}

void DrawScore(ScoreManager *score)
{
    ProgressBar bar = {0};
    char scoreText[20], accuracyText[20], comboText[20];
    static int scoreIncrease = 0;
    static float scoreTimer = 0;
    static int lastScore = 0;
    int alpha = (int)(255 * (scoreTimer / 1.5f));
    sprintf(scoreText, "%d", score->value);
    sprintf(accuracyText, "%.2f%%", score->ctx->score.accuracy);
    sprintf(comboText, "Combo\n Score!!\n %d", score->comboValue);
    // DrawRectangle(score->ctx->screen_width - score->width, 0, score->width, 10, BLACK);
    DrawRectangle(score->ctx->screen_width - score->width, bar.height + 70, score->width, score->height, WHITE);
    DrawRectangle(score->ctx->screen_width - score->width, bar.height + score->height + 100, score->width, score->height, WHITE);

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

    // DrawText("Score", score->ctx->screen_width - score->width + (score->width / 2) - (MeasureText("Score", 20) / 2), score->ctx->screen_height - score->height - 20, 20, WHITE);
    // DrawText(scoreText, score->ctx->screen_width - score->width + (score->width / 2) - (MeasureText(scoreText, 20) / 2), score->ctx->screen_height - score->height, 20, BLACK);

    // RLAPI void DrawTextPro(Font font, const char *text, Vector2 position, Vector2 origin, float rotation, float fontSize, float spacing, Color tint); // Draw text using Font and pro parameters (rotation)
    if (score->comboShow)
    {
        DrawTextPro(score->ctx->font, comboText, (Vector2) {0, 300}, (Vector2) {0, 0}, -15.0, 40, 1, Fade(WHITE, alpha / 255.0f));
    }
    
    // DrawTextPro(score->ctx->font, ComboScore, (Vector2) {0, 300}, (Vector2) {0, 0}, -15.0, 40, 1, WHITE);
    // DrawText(score->ctx->font, "Accuracy")
    DrawTextEx(score->ctx->font, "Accuracy", (Vector2) {score->ctx->screen_width - score->width + (score->width / 2) - (MeasureText("Accuracy", 16) / 2), bar.height + 50}, 19, 1, WHITE);
    DrawTextEx(score->ctx->font, accuracyText, (Vector2) {score->ctx->screen_width - score->width + (score->width / 2) - (MeasureText(accuracyText, 20) / 2), bar.height + 70}, 23, 1, BLACK);
    // DrawText("Accuracy", score->ctx->screen_width - score->width + (score->width / 2) - (MeasureText("Accuracy", 16) / 2), bar.height + 50, 16, WHITE);
    // DrawText(accuracyText, score->ctx->screen_width - score->width + (score->width / 2) - (MeasureText(accuracyText, 20) / 2), bar.height + 70, 20, BLACK);
    
    // DrawText("Score", score->ctx->screen_width - score->width + (score->width / 2) - (MeasureText("Score", 20) / 2), bar.height + score->height + 75, 20, WHITE);
    DrawTextEx(score->ctx->font, "Score", (Vector2) {score->ctx->screen_width - score->width + (score->width / 2) - (MeasureText("Score", 20) / 2), bar.height + score->height + 75}, 27, 1, WHITE);
    DrawTextEx(score->ctx->font, scoreText, (Vector2) {score->ctx->screen_width - score->width + (score->width / 2) - (MeasureText(scoreText, 20) / 2), bar.height + score->height + 100}, 23, 1, BLACK);
    // DrawText(scoreText, score->ctx->screen_width - score->width + (score->width / 2) - (MeasureText(scoreText, 20) / 2), bar.height + score->height + 100, 20, BLACK);

    // DrawText("Score Streak!!", score->ctx->screen_width - score->width + (score->width / 2) - (MeasureText("Score", 20) / 2), bar.height + score->height + 10, 20, WHITE);
    // DrawText(scoreText, score->ctx->screen_width - score->width + (score->width / 2) - (MeasureText(scoreText, 20) / 2), bar.height + score->height + 30, 20, BLACK);

    

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

        DrawText(scoreIncreaseText, score->ctx->screen_width - score->width + (score->width / 2) - (MeasureText(scoreIncreaseText, 20) / 2), bar.height + score->height + 125, 20, Fade(WHITE, alpha / 255.0f));

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
    score->ctx->score.perfect = score->perfect;
    score->ctx->score.good = score->good;
    score->ctx->score.miss = score->miss;

    if (IsSelectedMusicEnd(score->ctx))
    {
        SetScoreAndAccuracy(score->ctx, score->value, score->ctx->score.accuracy);
        // score->ctx->score.accuracy = 0.00;
        score->value = 0;
        score->isBeatmapLoaded = false;
        score->perfect = 0;
        score->good = 0;
        score->miss = 0;
        score->perfectCombo = 0;
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