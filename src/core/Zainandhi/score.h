#include "scene.h"
#include "context.h"
#include "gameplay.h"
#ifndef SCORE_H
#define SCORE_H

typedef enum Accuracy
{
    PERFECT,
    GOOD,
    MISS
} Accuracy;

typedef struct ScoreManager
{
    AppContext *ctx;
    int value, bestScore, comboValue;
    // double accuracy;
    int perfect, good, miss;
    
    int width, height;
    int perfectCombo;
    bool isBeatmapLoaded, comboShow;
    Beatmap beatmap;

    Texture2D fireFrames[3];
    int currentFrame;
    float frameTimer;
} ScoreManager;

ScoreManager InitScore(AppContext *ctx, Gameplay *gameplay);

void AddScore(ScoreManager *score, Accuracy acc);

void AddAcc(ScoreManager *score, Accuracy acc);

void DrawScore(ScoreManager *score);

void UpdateScore(ScoreManager *score);

bool IsShowScore(ScoreManager *score);

impl_scene(ScoreManager *, Score_ToScene, DrawScore, UpdateScore, IsShowScore);

#endif