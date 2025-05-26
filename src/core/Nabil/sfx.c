#include "sfx.h"
#include <stdio.h>

Sound introSfx;
Sound logoSfx;
Sound arrowSfx1;
Sound arrowSfx2;
Sound enterSfx;
Sound perfectSfx;
Sound goodSfx;
Sound missSfx;
Sound countSfx;

Sound SfxList[TOTAL_SOUND];
const char* soundFileNames[TOTAL_SOUND] = {
    "resources/sfx/1.coc-intro.wav",
    "resources/sfx/2.logo-sfx.wav",
    "resources/sfx/3.enter.wav",
    "resources/sfx/4.goodNote.wav",
    "resources/sfx/5.missNote.wav",
    "resources/sfx/6.perfectNote.wav",
    "resources/sfx/7.sfx1.wav",
    "resources/sfx/8.sfx2.wav",
    "resources/sfx/9.count.wav",
};
float sfxVolume[] = {
    0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
    1.0f, 0.5f, 1.0f, 0.5f
};
char *infoSfx[] = {
    "intro", "logo", "enter", "good",
    "miss", "perfect", "sfx1", "sfx2",
    "count"
};

SoundList SfxLinkedList = { NULL };
int methodCase;

// HARD CODE
void InitSfx(){
    methodCase = 1;

    introSfx = LoadSound("resources/sfx/1.coc-intro.wav");
    logoSfx = LoadSound("resources/sfx/2.logo-sfx.wav");
    enterSfx = LoadSound("resources/sfx/3.enter.wav");
    goodSfx = LoadSound("resources/sfx/4.goodNote.wav");
    missSfx = LoadSound("resources/sfx/5.missNote.wav");
    perfectSfx = LoadSound("resources/sfx/6.perfectNote.wav");
    arrowSfx1 = LoadSound("resources/sfx/7.sfx1.wav");
    arrowSfx2 = LoadSound("resources/sfx/8.sfx2.wav");
    countSfx = LoadSound("resources/sfx/9.count.wav");
    
    SetSoundVolume(introSfx, 0.5f); 
    SetSoundVolume(logoSfx, 0.5f); 
    SetSoundVolume(arrowSfx1, 0.5f); 
    SetSoundVolume(arrowSfx2, 0.5f); 
    SetSoundVolume(enterSfx, 0.5f);
    SetSoundVolume(missSfx, 1.0f); 
    SetSoundVolume(perfectSfx, 0.5f); 
    SetSoundVolume(goodSfx, 1.0f); 
    SetSoundVolume(countSfx, 0.5f); 
}

// ARRAY
void InitArraySfx(){
    methodCase = 2;

    for (int i = 0; i < TOTAL_SOUND; i++)
    {
        SfxList[i] = LoadSound(soundFileNames[i]);
        SetSoundVolume(SfxList[i], sfxVolume[i]);
    }
    logoSfx = SfxList[1];
}

// LINKED LIST
void InitLinkedListSfx(){
    SoundNode *newSound, *current, *temp;
    current = NULL;

    methodCase = 3;

    for (int i = 0; i < TOTAL_SOUND; i++)
    {
        newSound = (SoundNode*) malloc (sizeof(SoundNode));
        newSound->info = infoSfx[i];
        newSound->path = soundFileNames[i];
        newSound->sound = LoadSound(newSound->path);
        newSound->volume = sfxVolume[i];
        newSound->next = NULL;
        
        if (SfxLinkedList.head == NULL)
        {
            SfxLinkedList.head = newSound;
            current = newSound;
            newSound->next = SfxLinkedList.head;
        } else {
            current->next = newSound;
            newSound->next = SfxLinkedList.head;
            current = newSound;
        }
    }
    
    temp = SfxLinkedList.head;
    do {
        if (strcmp(temp->info, "logo") == 0) {
            logoSfx = temp->sound;
            break;
        }
        temp = temp->next;
    } while (temp != SfxLinkedList.head);
}

void LinkedListPlayHelper(char *info) {
    if (SfxLinkedList.head == NULL) return;

    SoundNode *node = SfxLinkedList.head;
    do {
        if (strcmp(info, node->info) == 0) {
            SetSoundVolume(node->sound, node->volume);
            PlaySound(node->sound);
            break;
        }
        node = node->next;
    } while (node != SfxLinkedList.head);
}

void LinkedListStopHelper(char *info) {
    SoundNode *node;
    node = SfxLinkedList.head;

    do {
        if (strcmp(info, node->info) == 0) {
            StopSound(node->sound);
            return;
        }
        node = node->next;
    } while (node != SfxLinkedList.head);
}

// PLAY SOUND OR CALL
void PlayIntroSfx() {
    switch (methodCase)
    {
    case 1:
        PlaySound(introSfx);
        break;
    case 2:
        PlaySound(SfxList[0]);
        break;
    case 3:
        LinkedListPlayHelper("intro");
        break;
    
    default:
        break;
    }
}

void PlayLogoSfx() {
    switch (methodCase)
    {
    case 1:
        PlaySound(logoSfx);
        break;
    case 2:
        PlaySound(SfxList[1]);
        break;
    case 3:
        LinkedListPlayHelper("logo");
        break;
    
    default:
        break;
    }
}

void StopLogoSfx() {
    switch (methodCase)
    {
    case 1:
        StopSound(logoSfx);
        break;
    case 2:
        StopSound(SfxList[1]);
        break;
    case 3:
        LinkedListStopHelper("logo");
        break;
    
    default:
        break;
    }
}

void PlayEnterSfx() {
    switch (methodCase)
    {
    case 1:
        PlaySound(enterSfx);
        break;
    case 2:
        PlaySound(SfxList[2]);
        break;
    case 3:
        LinkedListPlayHelper("enter");
        break;
    default:
        break;
    }
}

void PlayGoodSfx() {
    switch (methodCase)
    {
    case 1:
        PlaySound(goodSfx);
        break;
    case 2:
        PlaySound(SfxList[3]);
        break;
    case 3:
        LinkedListPlayHelper("good");
        break;
    
    default:
        break;
    }
}

void PlayMissSfx() {
    switch (methodCase)
    {
    case 1:
        PlaySound(missSfx);
        break;
    case 2:
        PlaySound(SfxList[4]);
        break;
    case 3:
        LinkedListPlayHelper("miss");
        break;
    
    default:
        break;
    }
}

void PlayPerfectSfx() {
    switch (methodCase)
    {
    case 1:
        PlaySound(perfectSfx);
        break;
    case 2:
        PlaySound(SfxList[5]);
        break;
    case 3:
        LinkedListPlayHelper("perfect");
        break;
    
    default:
        break;
    }
}


void PlayArrowSfx(int key) {
    if (key == KEY_D || key == KEY_F || key == KEY_A || key == KEY_W) {
        switch (methodCase)
        {
        case 1:
            PlaySound(arrowSfx1);
            break;
        case 2:
            PlaySound(SfxList[6]);
            break;
        case 3:
            LinkedListPlayHelper("sfx1");
            break;
        
        default:
            break;
        }
    } else if (key == KEY_K || key == KEY_J || key == KEY_D || key == KEY_S) {
        switch (methodCase)
        {
        case 1:
            PlaySound(arrowSfx2);
            break;
        case 2:
            PlaySound(SfxList[7]);
            break;
        case 3:
            LinkedListPlayHelper("sfx2");
            break;
        
        default:
            break;
        }
    }
}

void PlayCountScoreSfx(){
    switch (methodCase)
    {
    case 1:
        PlaySound(perfectSfx);
        break;
    case 2:
        PlaySound(SfxList[8]);
        break;
    case 3:
        LinkedListPlayHelper("count");
        break;
    
    default:
        break;
    }
}

void UnloadSfx() {
    switch (methodCase)
    {
    case 1:
        UnloadSound(arrowSfx1);
        UnloadSound(arrowSfx2);
        UnloadSound(enterSfx);
        UnloadSound(missSfx);
        UnloadSound(perfectSfx);
        UnloadSound(goodSfx);
        break;
    case 2: 
        for (int i = 0; i < TOTAL_SOUND; i++)
        {
            UnloadSound(SfxList[i]);
        }
        
    default:
        break;
    }
}