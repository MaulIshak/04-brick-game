#include "context.h"
#include <stdlib.h>
#include "macro.h"
#include <string.h>
#include <assert.h>

const char *resources[] = {
    "resources/Guardian of The Former Seas",
    "resources/Pest of The Cosmos",
};

Tracks InitTracks() {
    Tracks tr = {
        .cap = 10,
        .track = malloc(sizeof(Tracks) * 10),
    };

    int len = ARRAY_LEN(resources);
    tr.len = len;
    char buff[255] = {0};
    for(int i = 0; i < len; i++) {
        strcat(buff, resources[i]);
        strcat(buff, ".mp3");

        Music m = LoadMusicStream(buff);
        tr.track[i].music = m;
        strcpy(&tr.track[i].music_name, &buff[10]);
        buff[0] = '\0';
    }

    return tr;
}

void DestroyTracks(Tracks *tracks) {
    for(int i = 0; i < tracks->len; i++) {
        UnloadMusicStream(tracks->track[i].music);
    }
    free(tracks);
}

void DestroyContext(AppContext *ctx) {
    DestroyTracks(&ctx->tracks);
}

AppContext CreateContext(int screen_width , int screen_height ){
    Tracks tracks = InitTracks();
    AppContext ctx = {
        .app_state = APP_LOADING,
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
}

void StopSelectedTrack(AppContext *ctx) {
    int selected = ctx->selected_track;
    assert(selected != -1);
    StopMusicStream(ctx->tracks.track[selected].music);
    ctx->is_music_playing = false;
}

Beatmap GetSelectedMusicBeatmap(AppContext* ctx) {
    Beatmap map = {0};
    return map;
}

void SeekSelectedTrack(AppContext* ctx, float second) {
    size_t selected = ctx->selected_track;
    assert(SelectMusic != -1);

    SeekMusicStream(ctx->tracks.track[selected].music, second);
}