#include "sqlite3.h"
#include "context.h"
#include "stdio.h"
#include <stdlib.h>
#include "context.h"
#include "linked-list.h"
#include <assert.h>
#include <string.h>
#include "array_list.h"

char *music_table_query = "CREATE TABLE IF NOT EXISTS music (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, file TEXT);";
char *beatmap_table_query = "CREATE TABLE IF NOT EXISTS music_beatmap (id INTEGER PRIMARY KEY AUTOINCREMENT, music_id INTEGER, command INT, duration FLOAT, hit_at_ms FLOAT);";

char* music_score_table_query = "CREATE TABLE IF NOT EXISTS score (music_id INTEGER PRIMARY KEY, score INT, accuracy INT);"; 

char* select_beatmap_query = "SELECT * FROM music";
char* select_score_query = "SELECT * FROM score";

typedef int (*sqlite_callback)(void* userdata , int argc ,char** argv,char**colName);



void exec_query(sqlite3 *db, char* sql) {
    int err;
    char* msg;
    err = sqlite3_exec(db, sql, NULL, NULL, &msg );

    if(err != SQLITE_OK) {
        printf("%s",msg);
        exit(1);
    }
}

void select_query(sqlite3 *db, char* sql, sqlite_callback callback, void* userData) {
    int err;
    char* msg;
    err = sqlite3_exec(db, sql, callback, userData, &msg );

    if(err != SQLITE_OK) {
        printf("%s",msg);
        exit(1);
    }
}

void create_music_table(sqlite3 *db) {
    exec_query(db, music_table_query);
    return;
}


void create_beatmap_table(sqlite3 *db) {
    exec_query(db, beatmap_table_query);
    return;
}

void create_music_score_table(sqlite3 *db) {
    exec_query(db, music_score_table_query);
    return;
}

void insert_music_table(sqlite3 *db, char* music_name, char* file_name) {
    char buff[2048];
    sprintf(buff, "INSERT INTO music (name, file) VALUES ('%s', '%s')", music_name, file_name);
    exec_query(db, buff);
    return;
}

void insert_beatmap_table(sqlite3 *db, int music_id, int direction, int hit_at_ms, int duration_ms) {
    char buff[4069];
    sprintf(buff, "INSERT INTO music_beatmap (music_id, command, duration, hit_at_ms) VALUES (%d, %d, %d, %d)", music_id, direction, duration_ms, hit_at_ms);
    exec_query(db, buff);
    return;
}

// void insert_music_table(sqlite3 *db, char* music_name, char* file_name) {
//     char buff[2048];
//     sprintf(buff, "INSERT INTO music (name, file) VALUES ('%s', '%s')", music_name, file_name);
//     exec_query(db, buff);
//     return;
// }

int select_music_scanner(void* userdata, int argc ,char** argv ,char** colName) {
    Tracks *tracks = (Tracks*)userdata;
    Track *track;
    track = malloc(sizeof(Track));
    for(int i = 0; i < argc; i++) {
        
        char* value = argv[i];
        char* name = colName[i];

        if(strcmp("id", name) == 0) {
            track->music_id = atoi(value);
            tracks->len += 1;
            tracks->cap += 1;
        }

        if(strcmp("name", name) == 0) {
            strcpy(track->music_name, value);
        }

        if(strcmp("file", name) == 0) {
            printf("%s\n", value);
            track->music = LoadMusicStream(value);
            track->music.looping = false;
            track->file_path = strdup(value);
            int len = strlen(value);
            value[len] = '\0';
            value[len - 3] = 'p';
            value[len - 2] = 'n';
            value[len - 1] = 'g';
            track->cover = LoadTexture(value);
        }
        
    }
    
    node_append(&tracks->track, (opaque)track);
    return 0;
}

bool music_id_comparator(NodeInfoType n1, NodeInfoType n2){
    Track *left = (Track*)n1;
    Track *right = (Track*)n2;
    
    return left->music_id == right->music_id;
}

int select_score_scanner(void* userdata, int argc ,char** argv ,char** colName) {
    Track *track = NULL;
    Tracks *tracks = (Tracks*)userdata;
    
    for(int i = 0 ; i < argc; i++) {
        char* value = argv[i];
        char* name = colName[i];

        if(strcmp(name, "music_id") == 0) {
            int music_id = atoi(value);
            Track track_cmp = {0};
            track_cmp.music_id = music_id;
            track = (Track *)node_find(tracks->track, (opaque)&track_cmp, music_id_comparator)->info;
            track->music_id = music_id;
        }

        if(strcmp(name, "score") == 0) {
            assert(track != NULL);
            track->high_score = (int)atoi(value); 
        }

        if(strcmp(name, "accuracy") == 0) {
            assert(track != NULL);
            track->accuracy = (double)atof(value);
        }
    }
    return 0;
}

int select_beatmap_scanner(void* userdata, int argc ,char** argv ,char** colName) {
    Beatmap *beatmap = (Beatmap *)userdata;
    Note note = {0};
    for(int i = 0; i < argc; i++) {
        char* value = argv[i];
        char* name = colName[i];
        // command INT, duration FLOAT, hit_at_ms FLOAT
        if(strcmp(name, "command") == 0) {
            note.direction = atoi(value);
        }

        if(strcmp(name, "duration") == 0) {
            note.duration_in_ms = atof(value);
        } 

        if(strcmp(name, "hit_at_ms") == 0) {
            note.hit_at_ms = atof(value);
        }
    }

    da_append(beatmap, note);
    return 0;
}


void select_music(sqlite3 *db, Tracks *track){
    select_query(db, select_beatmap_query, select_music_scanner, track);
}

void populate_score(sqlite3 *db, Tracks *tracks) {
    for(int i = 0; i < tracks->len; i++) {
        Track* track = GetTrackRef(*tracks, i);
        track->high_score = 0;
        track->accuracy = 0;
    }
    select_query(db, select_score_query, select_score_scanner, tracks);
}

void get_beatmap(sqlite3 *db, int music_id, Beatmap *beatmap) {
    char buff[4096];
    sprintf(buff, "SELECT * FROM music_beatmap WHERE music_id = %d", music_id);
    select_query(db, buff, select_beatmap_scanner, beatmap);
    return;
}

void update_score(sqlite3 *db, int music_id, int score, float acc) {
    char buff[4096];
    sprintf(buff, "INSERT INTO score(music_id, score, accuracy) VALUES(%d, %d, %f) ON CONFLICT(music_id) DO UPDATE SET score = excluded.score, accuracy = excluded.accuracy;", music_id, score, acc ) ;
    exec_query(db, buff);
    return;
}