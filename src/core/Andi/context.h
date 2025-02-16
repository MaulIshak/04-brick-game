#include <raylib.h>
#include <stdlib.h>

#ifndef CONTEXT_H
#define CONTEXT_H

typedef enum NoteDirection {
    NOTE_LEFT,
    NOT_RIGHT,
    NOTE_UP,
    NOTE_DOWN,
} NoteDirection;

typedef struct Note {
    NoteDirection direction;
    double hit_at_ms;
} Note;

typedef struct Beatmap {
    Note *notes;
    int len,cap;
} Beatmap;

typedef struct Track {
    Music music;
    char music_name[50];
    int high_score;
} Track;

typedef struct Tracks {
    int len, cap;
    Track *track;
} Tracks;

typedef struct Score {
    long int point;
    double accuracy;
} Score;

Tracks InitTracks();

typedef enum State {
    APP_LOADING,
    APP_SELECT,
    APP_PLAYING,
    APP_PRESS_TO_PLAY,
} State;

typedef struct AppContext {
    int screen_width;
    int screen_height;
    State app_state;
    Tracks tracks;
    Score score;
    int selected_track;
    bool is_music_playing;
} AppContext;

AppContext CreateContext(int, int);
Beatmap GetSelectedMusicBeatmap(AppContext* ctx);
void DestroyTracks(Tracks *tracks);
void DestroyContext(AppContext *ctx);

void SelectMusic(AppContext* ctx);
void UpdateContext(AppContext* ctx);
void PlaySelectedTrack(AppContext* ctx);
void StopSelectedTrack(AppContext* ctx);

#endif // CONTEXT_H