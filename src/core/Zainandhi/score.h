<<<<<<< HEAD
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
    int value;
    // double accuracy;
    int perfect, good, miss;
    
    int width;
    int perfectCombo;
    bool isBeatmapLoaded;
    Beatmap beatmap;

    Texture2D fireFrames[3];
    int currentFrame;
    float frameTimer;
} ScoreManager;

ScoreManager InitScore(AppContext *ctx, Gameplay *gameplay);

void LoadingLoadTexturesFire(ScoreManager *score);

void LoadingUnloadTexturesFire(ScoreManager *score);

void AddScore(ScoreManager *score, Accuracy acc);

void DrawScore(ScoreManager *score);

void UpdateScore(ScoreManager *score);

bool IsShowScore(ScoreManager *score);

impl_scene(ScoreManager *, Score_ToScene, DrawScore, UpdateScore, IsShowScore);

=======
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
    int value;
    // double accuracy;
    
    int width;
    int perfectCombo;
    bool isBeatmapLoaded;
    Beatmap beatmap;

    Texture2D fireFrames[3];
    int currentFrame;
    float frameTimer;
} ScoreManager;

ScoreManager InitScore(AppContext *ctx, Gameplay *gameplay);

void LoadingLoadTexturesFire(ScoreManager *score);

void LoadingUnloadTexturesFire(ScoreManager *score);

void AddScore(ScoreManager *score, Accuracy acc);

void DrawScore(ScoreManager *score);

void UpdateScore(ScoreManager *score);

bool IsShowScore(ScoreManager *score);

impl_scene(ScoreManager *, Score_ToScene, DrawScore, UpdateScore, IsShowScore);

>>>>>>> d316daac2e809dcaad5220373f7d038c6530e06a
#endif