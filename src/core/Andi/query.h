#ifndef QUERY_H
#define QUERY_H

void create_music_table(sqlite3 *db);
void create_beatmap_table(sqlite3 *db);
void create_music_score_table(sqlite3 *db);
void insert_music_table(sqlite3 *db, char* music_name, char* file_name);

#endif // QUERY_H