#include "context.h"
#include <stdlib.h>
#include "macro.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

const char *resources[] = {
    "resources/Guardian of The Former Seas",
    "resources/Pest of The Cosmos",
};

Tracks InitTracks() {
    Tracks tr = {
        .cap = 10,
        .track = malloc(sizeof(Track) * 10),
    };

    int len = ARRAY_LEN(resources);
    tr.len = len;
    char buff[255] = {0};
    // TODO: parse dari file .map untuk ambil skor nya
    for(int i = 0; i < len; i++) {
        Track *track = &tr.track[i];
        strcat(buff, resources[i]);
        strcat(buff, ".mp3");

        Music m = LoadMusicStream(buff);
        track->music = m;
        track->high_score = 0;
        strcpy((char *)&track->music_name, (char *)&buff[10]);
        buff[0] = '\0';
    }

    return tr;
}

void DestroyTracks(Tracks *tracks) {
    for(int i = 0; i < tracks->len; i++) {
        UnloadMusicStream(tracks->track[i].music);
    }
    free(tracks->track);
}

void DestroyContext(AppContext *ctx) {
    DestroyTracks(&ctx->tracks);
}

AppContext CreateContext(int screen_width , int screen_height ){
    Tracks tracks = InitTracks();
    AppContext ctx = {
        .app_state = APP_PLAYING,
        .screen_width = screen_width,
        .screen_height = screen_height,
        .tracks = tracks,
        .selected_track = 1,
        .is_music_playing = false,
        .score = {0},
    };
    return ctx;
}

void UpdateContext(AppContext* ctx) {
    int selected = ctx->selected_track;
    if(ctx->is_music_playing) {
        assert(selected != -1);
        
        UpdateMusicStream(ctx->tracks.track[selected].music);
    }
}

void PlaySelectedTrack(AppContext *ctx) {
    int selected = ctx->selected_track ;
    assert(selected != -1);
    PlayMusicStream(ctx->tracks.track[selected].music);
    ctx->is_music_playing = true;
    #ifdef DEBUG
    printf("Playing %s\n",  ctx->tracks.track[selected].music_name);
    #endif
}

void StopSelectedTrack(AppContext *ctx) {
    int selected = ctx->selected_track;
    assert(selected != -1);
    StopMusicStream(ctx->tracks.track[selected].music);
    ctx->is_music_playing = false;
    #ifdef DEBUG
    printf("Stopping %s\n",  ctx->tracks.track[selected].music_name);
    #endif
}

Beatmap GetSelectedMusicBeatmap(AppContext* ctx) {
    // TODO: arraylist should be stored on context and resized when needed. free it on destroy context
    Beatmap map;
    map.notes = malloc(sizeof(Note) * 10);
    map.len = 10;
    map.cap = 10;

    for(int i = 0; i < 10; i++) {
        map.notes[i].direction = GetRandomValue(0,3);
        map.notes[i].hit_at_ms = (i + 2) + (500 * i);
        map.notes[i].position = (Vector2){0,0};
    }

    return map;
}

void SeekSelectedTrack(AppContext* ctx, float second) {
    size_t selected = ctx->selected_track;
    assert(ctx->selected_track != -1);

    SeekMusicStream(ctx->tracks.track[selected].music, second);
    #ifdef DEBUG
    printf("Seeking %s To %f seconds\n",  ctx->tracks.track[selected].music_name, second);
    #endif
}