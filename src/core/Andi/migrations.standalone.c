#define LINKED_LIST_TYPE void*
#define LINKED_LIST_IMPLEMENTATION
#include "linked-list.h"
#include "sqlite3.h"
#include "query.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "context.h"
#include "macro.h"
#include <string.h>
#include "raylib.h"



int main() {
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
    
    InitAudioDevice();
    InitWindow(800,600, "Migration on progres..");
    sqlite3 *db;
    char *msg;
    int err;
    NodeAddress cur;
    AppContext ctx = CreateContext(0,0);

    
    remove("beatmap.db");
    err = sqlite3_open("beatmap.db", &db);
    assert(err == SQLITE_OK);
    
    create_music_table(db);
    create_beatmap_table(db);

    
    for(int i = 0; i < ARRAY_LEN(music_lists); i++) {
        const char buff[2048] = {0};
        ctx.selected_track = i;
        Track track = GetSelectedTrack(&ctx);
        strcat(buff, "resources/");
        strcat(buff, track.music_name);
        strcat(buff, ".mp3");
        insert_music_table(db, track.music_name, buff);
        
        Beatmap map = GetSelectedMusicBeatmap(&ctx);

        for(int j = 0; j < map.len; j++) {
            insert_beatmap_table(db, i + 1, map.items[j].direction, map.items[j].hit_at_ms, map.items[j].duration_in_ms);
        }
    }

    return 0;
}