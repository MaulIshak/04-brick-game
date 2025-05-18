#ifndef SFX_H
#define SFX_H
#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#define TOTAL_SOUND 8

extern int methodCase;

extern Sound introSfx;
extern Sound logoSfx;
extern Sound arrowSfx1;
extern Sound arrowSfx2;
extern Sound enterSfx;
extern Sound perfectSfx;
extern Sound goodSfx;
extern Sound missSfx;

typedef struct
{
    char *info;
    Sound sound;
    float volume;
    char *path;
    struct SoundNode* next; 
} SoundNode;

typedef struct
{
    struct SoundNode* head; 
} SoundList;

void InitSfx();
void InitArraySfx();
void InitLinkedListSfx();
void LinkedListPlayHelper(char *info);
void LinkedListStopHelper(char *info);

void PlayIntroSfx();
void PlayLogoSfx();
void StopLogoSfx();
void PlayArrowSfx(int key);
void PlayEnterSfx();
void PlayPerfectSfx();
void PlayGoodSfx();
void PlayMissSfx();
void UnloadSfx();

#endif