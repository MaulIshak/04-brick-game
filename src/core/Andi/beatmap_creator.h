#pragma once

#include "context.h"
#include "timer.h"
#include "scene.h"

#ifndef BEATMAP_CREATOR_H
#define BEATMAP_CREATOR_H


typedef struct BeatmapCreator {
    AppContext* ctx;
    Timer timer;
    bool is_recording;
    Beatmap bmt;
    bool is_recording_session_end;
    bool is_map_written;
} BeatmapCreator;

void AppendNote(Beatmap* btm, Timer* timer, NoteDirection dir);

BeatmapCreator CreateBeatmap(AppContext* ctx);
void BeatmapCreator_Draw(BeatmapCreator* self);
void BeatmapCreator_Update(BeatmapCreator* self);
bool BeatmapCreator_IsShow(BeatmapCreator* self);

impl_scene(BeatmapCreator*, BeatmapCreator_ToScene, BeatmapCreator_Draw, BeatmapCreator_Update, BeatmapCreator_IsShow)

#endif// BEATMAP_CREATOR_H