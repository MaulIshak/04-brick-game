#include "context.h"
#include <stdlib.h>
#include "macro.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "array_list.h"
#include "sqlite3.h"
#include "query.h"

#include "linked-list.h"

const char *music_lists[] = {
    "resources/Outer Space",
    "resources/PKKMB 2024",
    "resources/Infinite Cosmodynamics",
    "resources/Guardian of The Former Seas",
    "resources/Pest of The Cosmos",
    "resources/LGR",
    "resources/ToyLand",
    "resources/Bad Apple",
};
// For bakcward compability
Track GetTrack(Tracks tracks, int index) {
    NodeInfoType add = node_at(tracks.track, index);
    Track* as_track = (Track*)add;
    return *as_track;
}

Track *GetTrackRef(Tracks tracks, int index) {
    NodeInfoType add = node_at(tracks.track, index);
    return (Track*)add;
}

Track GetSelectedTrack(AppContext* ctx) {
    return GetTrack(ctx->tracks, ctx->selected_track );
}

Tracks InitTracks() {
    NodeAddress add = NULL;
    Tracks tr = {
        .cap = 10,
        .len = 2,
        .track = NULL,
    };

    int len = ARRAY_LEN(music_lists);
    tr.len = len;
    char buff[255] = {0};
    for(int i = 0; i < len; i++) {
        Track *track = malloc(sizeof(Track));
        
        strcat(buff, music_lists[i]);
        strcat(buff, ".mp3");

        Music m = LoadMusicStream(buff);
        int len = strlen(buff);
        buff[len - 4] = '\0';
        m.looping = false;

        strcat(buff, ".map");
        GetScoreAndAccuracy(buff, &track->high_score, &track->accuracy);
        buff[len - 4] = '\0';

        track->music = m;

        strcat(buff, ".png");
        Texture tex = LoadTexture(buff);
        track->cover = tex;
        buff[len - 4] = '\0';

        strcpy((char *)&track->music_name, (char *)&buff[10]);
        buff[0] = '\0';
        node_append(&tr.track, (opaque)track);
        #ifdef DEBUG
        printf("Loaded %s | Score %d | Accuracy %f \n", track->music_name, track->high_score, track->accuracy);
        #endif
    }

    return tr;
}

Tracks InitTracksFromDB(sqlite3 *beatmap_db, sqlite3 *score_db) {
    Tracks tracks = {0};
    
    select_music(beatmap_db, &tracks);
    printf("MUSIC LOADED....\n");
    populate_score(score_db, &tracks);
    printf("SCORE POPULATED....\n");
    return tracks;
}

void GetScoreAndAccuracy(const char* file_name, int *scoreOut, float *accuracyOut){
    FILE *f = fopen(file_name, "r");
    char buff[2048];
    // buang nama musik
    fgets(buff, 2048, f);

    fgets(buff, 2048, f);
    *scoreOut = atoi(buff);
    
    fgets(buff, 2048, f);
    *accuracyOut = (float)atof(buff);

    fclose(f);
}


void DestroyTracks(Tracks *tracks) {
    for(int i = 0; i < tracks->len; i++) {
        Track track = GetTrack(*tracks, i);
        UnloadMusicStream(track.music);
        free(track.file_path);
    }
    free(tracks->track);
}

void DestroyContext(AppContext *ctx) {
    DestroyTracks(&ctx->tracks);
    free(ctx->_beatmap.items);
    free(ctx->_beatmap_name);
}
const char* font_path = "resources/font/Jersey15-Regular.ttf";

AppContext CreateForMigrate(int screen_width , int screen_height ){
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
    ctx._beatmap.items = malloc(sizeof(Note) * 10);
    ctx._beatmap.cap = 10;
    ctx._beatmap.len = 0;
    ctx._beatmap_name = malloc(sizeof(char) * 400);
    ctx._beatmap_music_id = -1;
    memset(ctx._beatmap_name, 0, 400);
    Font font = LoadFontEx(font_path, 30, NULL, 0);
    ctx.font = font;
    printf("Context created...\n");
    return ctx;
}

AppContext CreateContext(int screen_width , int screen_height ){
    sqlite3 *beatmap_db;
    sqlite3 *score_db;
    sqlite3_open("beatmap.db", &beatmap_db);
    sqlite3_open("score.db", &score_db);
    create_music_score_table(score_db);
    Tracks tracks = InitTracksFromDB(beatmap_db, score_db);
    // Tracks tracks = InitTracks();
    AppContext ctx = {
        .app_state = APP_LOADING,
        .screen_width = screen_width,
        .screen_height = screen_height,
        .tracks = tracks,
        .selected_track = 1,
        .is_music_playing = false,
        .score = {0},
        .beatmap_db = beatmap_db,
        .score_db = score_db,
    };
    ctx._beatmap.items = malloc(sizeof(Note) * 10);
    ctx._beatmap.cap = 10;
    ctx._beatmap.len = 0;
    ctx._beatmap_name = malloc(sizeof(char) * 400);
    ctx._beatmap_music_id = -1;
    memset(ctx._beatmap_name, 0, 400);
    Font font = LoadFontEx(font_path, 30, NULL, 0);
    ctx.font = font;
    printf("Context created...\n");
    return ctx;
}

void UpdateContext(AppContext* ctx) {
    int selected = ctx->selected_track;
    if(ctx->is_music_playing) {
        assert(selected != -1);
        Track track = GetTrack(ctx->tracks, selected);
        UpdateMusicStream(track.music);
    }
}

void PlaySelectedTrack(AppContext *ctx) {
    int selected = ctx->selected_track;
    assert(selected != -1);
    Track track = GetTrack(ctx->tracks, selected);
    PlayMusicStream(track.music);
    SeekSelectedTrack(ctx, 0.01);
    ctx->is_music_playing = true;
    #ifdef DEBUG
    printf("PlaySelectedTrack: Playing %s\n",  track.music_name);
    #endif
}

void StopSelectedTrack(AppContext *ctx) {
    int selected = ctx->selected_track;
    assert(selected != -1);
    Track track = GetTrack(ctx->tracks, selected);
    StopMusicStream(track.music);
    ctx->is_music_playing = false;
    #ifdef DEBUG
    printf("StopSelectedTrack: Stopping %s\n",  track.music_name);
    #endif
}

Beatmap GetSelectedMusicBeatmap(AppContext* ctx) {
    Track track = GetSelectedTrack(ctx);
    if(ctx->_beatmap_music_id != track.music_id) {
        ctx->_beatmap.len = 0;
        ctx->_beatmap_music_id = track.music_id;
        get_beatmap(ctx->beatmap_db, track.music_id, &ctx->_beatmap);
        printf("GetSelectedMusicBeatmap: Readed %d notes\n", ctx->_beatmap.len);
        
    }else {
        printf("GetSelectedMusicBeatmapDB: CACHE HIT");
        printf("GetSelectedMusicBeatmap: Readed %d notes\n", ctx->_beatmap.len);
    }
    return ctx->_beatmap;
}

Beatmap GetSelectedMusicBeatmapForDB(AppContext* ctx) {

    char *music_name = GetSelectedMusicName(ctx);
    if (strcmp(ctx->_beatmap_name, music_name ) == 0) {
        printf("GetSelectedMusicBeatmap: CACHE HIT");
        return ctx->_beatmap;
    } else {
        strcpy(ctx->_beatmap_name, music_name);
    }
    ctx->_beatmap.len = 0;
    int selected = ctx->selected_track;
    assert(selected != -1);
    char buff[2048] = {0};
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
        printf("%s", num_text);
        char *ms_text = strtok(NULL, " ");
        printf("%s", ms_text);
        char *duration_text = strtok(NULL, "\r\n");
        printf("%s", duration_text);
        if(ms_text == NULL) {
            break;
        }
        int dir = atoi(num_text);
        int ms = atoi(ms_text);
        int duration = atoi(duration_text);
        Note not = {
            .direction = dir,
            .hit_at_ms = ms,
            .position = (Vector2){0,0},
            .duration_in_ms = duration,
        };

        da_append(&ctx->_beatmap, not);
    }
    #ifdef DEBUG
    printf("GetSelectedMusicBeatmap: Readed %d notes\n", ctx->_beatmap.len);
    printf("----------------------------------\n");
    for(int i = 0; i < ctx->_beatmap.len; i++) {
        printf("\tDIRECTION %d MS %f DURATION %f\n", ctx->_beatmap.items[i].direction, ctx->_beatmap.items[i].hit_at_ms, ctx->_beatmap.items[i].duration_in_ms);
    }
    printf("----------------------------------\n");
    #endif
    fclose(f);

    return ctx->_beatmap;
}

void SeekSelectedTrack(AppContext* ctx, float second) {
    size_t selected = ctx->selected_track;
    assert(ctx->selected_track != -1);
    Track track = GetTrack(ctx->tracks, selected);
    SeekMusicStream(track.music, second);
    #ifdef DEBUG
    printf("Seeking %s To %f seconds\n",  track.music_name, second);
    #endif
}

bool IsSelectedMusicEnd(AppContext* ctx) {
    size_t selected = ctx->selected_track;
    assert(ctx->selected_track != -1);
    Track track = GetTrack(ctx->tracks, selected);
    float time_played = GetMusicTimePlayed(track.music);
    return time_played == 0 ;
}

char *GetSelectedMusicName(AppContext* ctx) {
    size_t selected = ctx->selected_track;
    assert(ctx->selected_track != -1);
    Track *track = GetTrackRef(ctx->tracks, selected);
    return track->music_name;
}

void WriteSelectedMusicBeatmapToFile(Beatmap* btm, const char* music_name, int score, float accuracy){
    char buff[2048] = {0};
    Beatmap map = *btm;
    strcat(buff, "resources/");
    strcat(buff, music_name);
    strcat(buff, ".map");
    FILE *f = fopen(buff, "w");
    // setup nama musik, skor, akurasi.
    fprintf(f, "%s\n%d\n%f\n", music_name, score, accuracy);
    
    for(int i = 0; i < btm->len; i++) {
        int dir = (int)btm->items[i].direction;
        int hit_at = (int)btm->items[i].hit_at_ms;
        int duration = (int)btm->items[i].duration_in_ms;
        fprintf(f, "%d %d %d\n", dir, hit_at, duration);
    }
    fclose(f);
}

void SetScoreAndAccuracy(AppContext* ctx, int score, int acc){
    int selected = ctx->selected_track;
    Track track = GetTrack(ctx->tracks, selected);
    update_score(ctx->score_db, track.music_id ,score, (float)acc);
    populate_score(ctx->score_db, &ctx->tracks);
}

float GetSelectedMusicLength(AppContext* ctx){
    int selected = ctx->selected_track;
    Track track = GetTrack(ctx->tracks, selected);
    Music music = track.music;
    return GetMusicTimeLength(music); 
}
float GetSelectedMusicTimePlayed(AppContext* ctx){
    int selected = ctx->selected_track;
    Track track = GetTrack(ctx->tracks, selected);
    Music music = track.music;
    return GetMusicTimePlayed(music);
}