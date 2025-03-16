<<<<<<< HEAD
#pragma once
#include <raylib.h>
#include "scene.h"
#include "context.h"

#ifndef SELECTION_MENU_H
#define SELECTION_MENU_H
#define MAX_MUSIC_SHOWING 30
#define FRAME_COUNT 9
typedef struct SelectionMenu {
    AppContext *ctx;

    float rotationOffset;
    float targetRotationOffset;
    
    float selectionYOffset;
    float targetYOffset;
    
    bool isShow;
} SelectionMenu;

static void AudioProcessEffectLPF(void *buffer, unsigned int frames);

void SelectionMenu_Draw(SelectionMenu *self);
void SelectionMenu_Update(SelectionMenu *self);
bool SelectionMenu_IsShow(SelectionMenu *self);
void InitSelectionMenu(SelectionMenu *selection_menu, AppContext *ctx);

impl_scene(SelectionMenu*, SelectionMenu_ToScene, SelectionMenu_Draw, SelectionMenu_Update, SelectionMenu_IsShow)

=======
#pragma once
#include <raylib.h>
#include "scene.h"
#include "context.h"

#ifndef SELECTION_MENU_H
#define SELECTION_MENU_H

typedef struct SelectionMenu {
    AppContext *ctx;
    int count;
    int selected;

    float rotationOffset;
    float targetRotationOffset;
    
    float selectionYOffset;
    float targetYOffset;
    
    float fontsize;
} SelectionMenu;


void SelectionMenu_Draw(SelectionMenu *self);
void SelectionMenu_Update(SelectionMenu *self);
bool SelectionMenu_IsShow(SelectionMenu *self);


impl_scene(SelectionMenu*, SelectionMenu_ToScene, SelectionMenu_Draw, SelectionMenu_Update, SelectionMenu_IsShow)

>>>>>>> e48a901611dc93d1cd8bd926622f7b08869c2ee8
#endif // SELECTION_MENU_H