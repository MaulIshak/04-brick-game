/**
 * beatmap_creator.h
 * 
 * pembuat: andiputraw
 * Modul ini adalah modul yang digunakan pada tampilan Beatmap Creation.
 * 
 */
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
    Timer up_timer, down_timer, left_timer, right_timer;
} BeatmapCreator;
// Menambah note ke beatmap
void AppendNote(Beatmap* btm, Timer* timer, NoteDirection dir, Timer *duration_timer);

// Membuat struct beatmap
BeatmapCreator CreateBeatmap(AppContext* ctx);
// Menggambar beatmap
void BeatmapCreator_Draw(BeatmapCreator* self);
// mengupdate beatmap
void BeatmapCreator_Update(BeatmapCreator* self);
// pengecekan jika beatmap perlu digambar atau tidak
bool BeatmapCreator_IsShow(BeatmapCreator* self);

impl_scene(BeatmapCreator*, BeatmapCreator_ToScene, BeatmapCreator_Draw, BeatmapCreator_Update, BeatmapCreator_IsShow)

#endif// BEATMAP_CREATOR_H