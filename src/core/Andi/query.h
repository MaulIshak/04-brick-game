/*
    query.h
    pembuat: andiputraw


    Modul-modul yang tersedia disini adalah modul untuk melakukan query ke database sqlite.
*/
#ifndef QUERY_H
#define QUERY_H

#include "context.h"

// membuat tabel musik
void create_music_table(sqlite3 *db);
// membuat tabel beatmap
void create_beatmap_table(sqlite3 *db);
// membuat tabel skor musik
void create_music_score_table(sqlite3 *db);
// insert value musik table
void insert_music_table(sqlite3 *db, char* music_name, char* file_name);
// insert value beatmap table
void insert_beatmap_table(sqlite3 *db, int music_id, int direction, int hit_at_ms, int duration_ms);
// populasi track dari db ke linked list
void select_music(sqlite3 *db, Tracks *track);
// populasi skor kedalam track
void populate_score(sqlite3 *db, Tracks *track);
// Update skor terbaru ke db
void update_score(sqlite3 *db, int music_id, int score, float acc);
// ambil beatmap dari DB
void get_beatmap(sqlite3 *db, int music_id, Beatmap *beatmap) ;

#endif // QUERY_H