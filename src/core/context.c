// #include "context.h"
// #include <stdlib.h>
// #include "macro.h"
// #include "string.h"

// const char *resources[] = {
//     "resources/Guardian of The Former Seas",
//     "resources/Pest of The Cosmos",
// };

// Tracks InitTracks() {
//     Tracks tr = {
//         .cap = 10,
//         .track = malloc(sizeof(Tracks) * 10),
//     };

//     int len = ARRAY_LEN(resources);
//     tr.len = len;
//     char buff[255] = {0};
//     for(int i = 0; i < len; i++) {
//         strcat(buff, resources[i]);
//         strcat(buff, ".mp3");

//         Music m1 = LoadMusicStream(buff);
//         buff[0] = '\0';
//     }

//     return tr;
// }

// void PlaySelectedTrack() {
    
// }