#include "context.h"
#include <stdlib.h>
#include "macro.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "array_list.h"

const char *resources[] = {
    "resources/Guardian of The Former Seas",
    "resources/Pest of The Cosmos",
    "resources/LGR",
};

Tracks InitTracks() {
    Tracks tr = {
        .cap = 10,
        .len = 2,
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
        int len = strlen(buff);
        buff[len - 4] = '\0';
        m.looping = false;
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
    free(ctx->_beatmap.items);
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
    ctx._beatmap.items = malloc(sizeof(Note) * 10);
    ctx._beatmap.cap = 10;
    ctx._beatmap.len = 0;
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
    SeekSelectedTrack(ctx, 0.01);
    ctx->is_music_playing = true;
    #ifdef DEBUG
    printf("PlaySelectedTrack: Playing %s\n",  ctx->tracks.track[selected].music_name);
    #endif
}

void StopSelectedTrack(AppContext *ctx) {
    int selected = ctx->selected_track;
    assert(selected != -1);
    StopMusicStream(ctx->tracks.track[selected].music);
    ctx->is_music_playing = false;
    #ifdef DEBUG
    printf("StopSelectedTrack: Stopping %s\n",  ctx->tracks.track[selected].music_name);
    #endif
}

Beatmap GetSelectedMusicBeatmap(AppContext* ctx) {
    ctx->_beatmap.len = 0;
    int selected = ctx->selected_track;
    assert(selected != -1);
    char buff[2048] = {0};
    char *music_name = GetSelectedMusicName(ctx);
    strcat(buff, "resources/");
    strcat(buff, music_name);
    strcat(buff, ".map");
    
    FILE *f = fopen(buff, "r");
    // buang metadata
    for(int i = 0; i < 3; i++) fgets(buff, 2048, f);
    
    for(int i = 0; ; i++) {
        fgets(buff, 2048, f);
      
        int i = 0;
        char *c = &buff[i];
        char *num_text = strtok(c, " ");
        char *ms_text = strtok(NULL, "\r\n");
        if(ms_text == NULL) {
            break;
        }
        int dir = atoi(num_text);
        int ms = atoi(ms_text);
        Note not = {
            .direction = dir,
            .hit_at_ms = ms,
            .position = (Vector2){0,0},
        };

        da_append(&ctx->_beatmap, not);
    }
    #ifdef DEBUG
    printf("GetSelectedMusicBeatmap: Readed %d notes\n", ctx->_beatmap.len);
    printf("----------------------------------\n");
    for(int i = 0; i < ctx->_beatmap.len; i++) {
        printf("\tDIRECTION %d MS %f\n", ctx->_beatmap.items[i].direction, ctx->_beatmap.items[i].hit_at_ms);
    }
    printf("----------------------------------\n");
    #endif
    fclose(f);

    return ctx->_beatmap;
}

void SeekSelectedTrack(AppContext* ctx, float second) {
    size_t selected = ctx->selected_track;
    assert(ctx->selected_track != -1);

    SeekMusicStream(ctx->tracks.track[selected].music, second);
    #ifdef DEBUG
    printf("Seeking %s To %f seconds\n",  ctx->tracks.track[selected].music_name, second);
    #endif
}

bool IsSelectedMusicEnd(AppContext* ctx) {
    size_t selected = ctx->selected_track;
    assert(ctx->selected_track != -1);
    float time_played = GetMusicTimePlayed(ctx->tracks.track[selected].music);
    return time_played == 0 ;
}

char *GetSelectedMusicName(AppContext* ctx) {
    size_t selected = ctx->selected_track;
    assert(ctx->selected_track != -1);
    return ctx->tracks.track[selected].music_name;
}