#include "sqlite3.h"
#include "stdio.h"
#include <stdlib.h>

char *music_table_query = "CREATE TABLE IF NOT EXISTS music (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, file TEXT);";
char *beatmap_table_query = "CREATE TABLE IF NOT EXISTS music_beatmap (id INTEGER PRIMARY KEY AUTOINCREMENT, music_id INTEGER, command INT, time FLOAT);";

char* music_score_table_query = "CREATE TABLE IF NOT EXISTS score (music_id INTEGER, score INT, accuracy INT);"; 

void exec_query(sqlite3 *db, char* sql) {
    int err;
    char* msg;
    err = sqlite3_exec(db, sql, NULL, NULL, &msg );

    if(err != SQLITE_OK) {
        printf(msg);
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
    sprintf(buff, "INSERT INTO beatmap (music_id, command, time)");
}

// void insert_music_table(sqlite3 *db, char* music_name, char* file_name) {
//     char buff[2048];
//     sprintf(buff, "INSERT INTO music (name, file) VALUES ('%s', '%s')", music_name, file_name);
//     exec_query(db, buff);
//     return;
// }