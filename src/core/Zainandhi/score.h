#include "scene.h"
#include "context.h"
#ifndef SCORE_H
#define SCORE_H


typedef struct ScoreManage
{
    AppContext* ctx;
    int value;
}ScoreManage;

ScoreManage InitScore(AppContext *ctx);

void AddScore(ScoreManage *score, int amount);

void DrawScore(ScoreManage *score);

void UpdateScore(ScoreManage *score);

bool IsShowScore(ScoreManage *score);

impl_scene(ScoreManage*, Score_ToScene, DrawScore, UpdateScore, IsShowScore);

#endif