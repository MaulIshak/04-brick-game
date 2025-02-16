#include <raylib.h>
#include <stdlib.h>

#ifndef CONTEXT_H
#define CONTEXT_H

typedef struct Track {
    Music music;
    char music_name[50];
    int high_score;
} Track;

typedef struct Tracks {
    int len, cap;
    Track *track;
} Tracks;

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
    int selected_track;
} AppContext;

void PlaySelectedTrack(AppContext* ctx);
void StopSelectedTrack(AppContext* ctx);

#endif // CONTEXT_H