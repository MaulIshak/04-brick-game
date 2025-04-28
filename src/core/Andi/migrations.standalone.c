#include "sqlite3.h"

int main() {
    sqlite3 *db;
    char *msg;
    int err;

    sqlite3_open("beatmap.db", &db);

    err = sqlite3_exec(db, "", NULL, NULL, &msg);

    if(err != SQLITE_OK) {
        printf("msg: %s", msg);
        exit(1);
    }

    return 0;
}