#include "sqlite3.h"
#include "query.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "context.h"
#include "macro.h"
#include <string.h>
#ifndef LINKED_LIST_TYPE void*
#define LINKED_LIST_IMPLEMENTATION
#include "linked-list.h"



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
    
    sqlite3 *db;
    char *msg;
    int err;
    NodeAddress cur;
    AppContext ctx = CreateContext(0,0);

    

    err = sqlite3_open("beatmap.db", &db);
    assert(err == SQLITE_OK);
    
    create_music_table(db);
    create_beatmap_table(db);

    
    for(int i = 0; i < ARRAY_LEN(music_lists); i++) {
        ctx.selected_track = i;
        Track track = GetSelectedTrack();
        insert_music_table(db, music_name, music_path_buff);

        ctx.
        
    }

    return 0;
}