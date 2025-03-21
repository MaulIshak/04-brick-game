#include "sfx.h"
#include <stdio.h>

Sound arrowSfx1;
Sound arrowSfx2;
Sound enterSfx;
Sound perfectSfx;
Sound goodSfx;
Sound missSfx;

void InitSfx() {
    arrowSfx1 = LoadSound("resources/sfx/sfx2.wav");
    arrowSfx2 = LoadSound("resources/sfx/sfx1.wav");
    enterSfx = LoadSound("resources/sfx/enter.wav");
    perfectSfx = LoadSound("resources/sfx/perfectNote.wav");
    goodSfx = LoadSound("resources/sfx/goodNote.wav");
    missSfx = LoadSound("resources/sfx/missNote.wav");

    SetSoundVolume(arrowSfx1, 0.5f); 
    SetSoundVolume(arrowSfx2, 0.5f); 
    SetSoundVolume(enterSfx, 0.5f);
    SetSoundVolume(missSfx, 1.0f); 
    SetSoundVolume(perfectSfx, 0.5f); 
    SetSoundVolume(goodSfx, 1.0f); 
}

void UnloadSfx() {
    UnloadSound(arrowSfx1);
    UnloadSound(arrowSfx2);
    UnloadSound(enterSfx);
    UnloadSound(missSfx);
    UnloadSound(perfectSfx);
    UnloadSound(goodSfx);
}

void PlayArrowSfx(int key) {
    if (key == KEY_LEFT || key == KEY_UP || key == KEY_A || key == KEY_W) {
        PlaySound(arrowSfx1);
    } else if (key == KEY_RIGHT || key == KEY_DOWN || key == KEY_D || key == KEY_S) {
        PlaySound(arrowSfx2);
    }
}

void PlayEnterSfx() {
    PlaySound(enterSfx);
}

void PlayMissSfx() {
    PlaySound(missSfx);
}

void PlayPerfectSfx() {
    PlaySound(perfectSfx);
}

void PlayGoodSfx() {
    PlaySound(goodSfx);
}