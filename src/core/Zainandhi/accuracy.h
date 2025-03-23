#include "scene.h"
#include "context.h"
#include "gameplay.h"
#include "score.h"
#ifndef ACCURACY_H
#define ACCURACY_H

ScoreManager InitAcc(AppContext *ctx, Gameplay *gameplay);

void DrawAcc(ScoreManager *score);

void UpdateAcc(ScoreManager *score);

bool IsShowAcc(ScoreManager *score);

impl_scene(ScoreManager *, Acc_ToScene, DrawAcc, UpdateAcc, IsShowAcc);

#endif