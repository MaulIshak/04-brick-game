#include "scene.h"
#include "context.h"
#include "gameplay.h"
#include "note.h"
#ifndef SCORE_H
#define SCORE_H


typedef struct ScoreManager
{
    AppContext* ctx;
    int value;
    int width;
    bool isBeatmapLoaded;
    Beatmap beatmap;
}ScoreManager;

ScoreManager InitScore(AppContext *ctx, Gameplay *gameplay);

void AddScore(ScoreManager *score, Accuracy acc);

void DrawScore(ScoreManager *score);

void UpdateScore(ScoreManager *score);

bool IsShowScore(ScoreManager *score);

impl_scene(ScoreManager*, Score_ToScene, DrawScore, UpdateScore, IsShowScore);

#endif