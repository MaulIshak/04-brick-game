#ifndef SFX_H
#define SFX_H
#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#define TOTAL_SOUND 9

// Mengdeklarasi variable tetapi tidak langsung diisi
extern int methodCase;
extern Sound introSfx;
extern Sound logoSfx;
extern Sound arrowSfx1;
extern Sound arrowSfx2;
extern Sound enterSfx;
extern Sound perfectSfx;
extern Sound goodSfx;
extern Sound missSfx;
extern Sound countSfx;

// Struct SoundNode
typedef struct
{
    char *info;
    Sound sound;
    float volume;
    char *path;
    struct SoundNode* next; 
} SoundNode;

// Struct head untuk sound node
typedef struct
{
    struct SoundNode* head; 
} SoundList;

// Modul untuk inisialisasi atau pemberian nilai awal
void InitSfx();
void InitArraySfx();
void InitLinkedListSfx();

// Modul helper untuk play sound yang kondisional
void LinkedListPlayHelper(char *info);
void LinkedListStopHelper(char *info);

// Modul untuk melakukan play sound
void PlayIntroSfx();
void PlayLogoSfx();
void StopLogoSfx();
void PlayArrowSfx(int key);
void PlayEnterSfx();
void PlayPerfectSfx();
void PlayGoodSfx();
void PlayMissSfx();
void PlayCountScoreSfx();

// Modul untuk unload sound
void UnloadSfx();

#endif