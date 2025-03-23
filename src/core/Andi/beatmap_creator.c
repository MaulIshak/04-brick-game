#include "beatmap_creator.h"
#include "timer.h"
#include "context.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include "array_list.h"
#include "string.h"

BeatmapCreator CreateBeatmap(AppContext* ctx){
    BeatmapCreator bt = {0};
    bt.ctx = ctx;
    bt.is_recording = false;
    bt.bmt.items = malloc(sizeof(Note) * 10);
    bt.bmt.cap = 10;
    bt.bmt.len = 0;
    bt.is_recording_session_end = false;
    return bt;
}

void BeatmapCreator_Draw(BeatmapCreator* self){
    double time = time_elapsed(&self->timer);
    char buff[256] = {0};
    if (time < 0) { 
        int s_int = (int)fabs(ceil(time));
        sprintf(buff, "%d", s_int );
        DrawText(buff, self->ctx->screen_width / 2, 10, 20, GRAY);
    }else {
        int ms = (int)s_to_ms(time_elapsed(&self->timer));
        sprintf(buff, "Notes: %d |", self->bmt.len);
        int notes_len = MeasureText(buff, 20);
        DrawText("Recording...", self->ctx->screen_width / 2 - MeasureText("Recording...", 20) / 2, 10, 20, GRAY);
        DrawText(buff, (self->ctx->screen_width / 2) - notes_len - 4, 40, 20, GRAY);
        sprintf(buff, "ms: %06d", ms);
        DrawText(buff, (self->ctx->screen_width / 2), 40, 20, GRAY);
    }

    if(self->is_map_written) {
        const char *text = "Map created. Press \"Q\" to go back to the title screen.";
        DrawText(text, self->ctx->screen_width / 2 - MeasureText(text, 20) / 2, self->ctx->screen_height / 2, 20, GRAY);
    }

    if(IsKeyDown(KEY_D)){
        DrawRectangle(self->ctx->screen_width/ 5 * 1, self->ctx->screen_height / 2 , 40, 40, RED);
    }
    if(IsKeyDown(KEY_F)){
        DrawRectangle(self->ctx->screen_width/ 5 * 2, self->ctx->screen_height / 2 , 40, 40, RED);
    }
    if(IsKeyDown(KEY_K)){
        DrawRectangle(self->ctx->screen_width/ 5 * 3, self->ctx->screen_height / 2 , 40, 40, RED);
    }
    if(IsKeyDown(KEY_J)){
        DrawRectangle(self->ctx->screen_width/ 5 * 4, self->ctx->screen_height / 2 , 40, 40, RED);
    }
}

void BeatmapCreator_Update(BeatmapCreator* self){
    if(!self->is_recording) {
        if(!self->timer.is_started) {
            timer_start(&self->timer, 3);
        }

        if(is_timer_end(&self->timer)){
            PlaySelectedTrack(self->ctx);
            self->is_recording = true;
            timer_start(&self->timer, 0);
        }
    }else if(self->is_recording && !self->is_recording_session_end) {
        if(!IsSelectedMusicEnd(self->ctx)){
            if(IsKeyPressed(KEY_D)) AppendNote(&self->bmt, &self->timer, NOTE_LEFT);
            if(IsKeyPressed(KEY_F)) AppendNote(&self->bmt, &self->timer, NOTE_UP);
            if(IsKeyPressed(KEY_J)) AppendNote(&self->bmt, &self->timer, NOTE_DOWN);
            if(IsKeyPressed(KEY_K)) AppendNote(&self->bmt, &self->timer, NOTE_RIGHT);
        }else {
            StopSelectedTrack(self->ctx);
            self->is_recording_session_end = true;
        }
    }

    if(self->is_recording_session_end && !self->is_map_written){
        WriteSelectedMusicBeatmapToFile(&self->bmt, GetSelectedMusicName(self->ctx), 0, 0.0);
        self->is_map_written = true;
    }

    if(self->is_map_written) {
        if(IsKeyPressed(KEY_Q)) {
            self->ctx->app_state = APP_PRESS_TO_PLAY;
        }
    }
}

void AppendNote(Beatmap* btm, Timer* timer, NoteDirection dir){
    double ms = s_to_ms(time_elapsed(timer));
    Note note = {
        .direction = dir,
        .hit_at_ms = ms - 100,
        .position = (Vector2){0,0},
    };
    da_append(btm, note);
}

bool BeatmapCreator_IsShow(BeatmapCreator* self){
    if(self->ctx->app_state == APP_BEATMAP_CREATOR) {
        return true;
    }
    return false;
}