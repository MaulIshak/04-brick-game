#ifndef QUERY_H
#define QUERY_H

void create_music_table(sqlite3 *db);
void create_beatmap_table(sqlite3 *db);
void create_music_score_table(sqlite3 *db);
void insert_music_table(sqlite3 *db, char* music_name, char* file_name);
void insert_beatmap_table(sqlite3 *db, int music_id, int direction, int hit_at_ms, int duration_ms);
void select_music(sqlite3 *db, Tracks *track);
void populate_score(sqlite3 *db, Tracks *track);
void update_score(sqlite3 *db, int music_id, int score, float acc);
void get_beatmap(sqlite3 *db, int music_id, Beatmap *beatmap) ;

#endif // QUERY_H