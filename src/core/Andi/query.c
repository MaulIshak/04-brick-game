#include "sqlite3.h"
#include "stdio.h"
#include <stdlib.h>

char *music_table_query = "CREATE IF NOT EXISTS TABLE music (id INT PRIMARY KEY AUTOINCREMENT, name TEXT, file TEXT);";
char *beatmap_table_query = "CREATE IF NOT EXISTS TABLE beatmap (id INT PRIMARY KEY AUTOINCREMENT, music_id INT, command INT, time FLOAT);";

char* music_score_table_query = "CREATE IF NOT EXISTS TABLE beatmap (music_id INT, score INT, accuracy INT);"; 

void create_table_query(sqlite3 *db, char* sql) {
    int err;
    char* msg;
    err = sqlite3_exec(db, sql, NULL, NULL, &msg );

    if(err != SQLITE_OK) {
        printf(msg);
        exit(1);
    }
}

void create_music_table(sqlite3 *db) {
    create_table_query(db, music_table_query);
    return;
}


void create_beatmap_table(sqlite3 *db) {
    create_table_query(db, beatmap_table_query);
    return;
}

void create_music_score_table(sqlite3 *db) {
    create_table_query(db, music_score_table_query);
    return;
}