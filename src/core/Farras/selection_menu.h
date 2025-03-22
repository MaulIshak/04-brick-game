#pragma once
#include <raylib.h>
#include "scene.h"
#include "context.h"
#include "flying_object.h"

#ifndef SELECTION_MENU_H
#define SELECTION_MENU_H
#define MAX_MUSIC_SHOWING 30
typedef struct SelectionMenu {
    AppContext *ctx;

    float rotationOffset;
    float targetRotationOffset;
    
    float selectionYOffset;
    float targetYOffset;
    
    bool isShow;

    FlyingObjects flying_objects;
    Texture2D background;
} SelectionMenu;

void PressToAction(SelectionMenu *self);
void PreviewMusic(SelectionMenu *self);
void SelectionMenuItem_Draw(SelectionMenu *self);
void SelectionMenuHelper_Draw(SelectionMenu *self);

void SelectionMenu_Draw(SelectionMenu *self);
void SelectionMenu_Update(SelectionMenu *self);
bool SelectionMenu_IsShow(SelectionMenu *self);

void InitSelectionMenu(SelectionMenu *selection_menu, AppContext *ctx);

impl_scene(SelectionMenu*, SelectionMenu_ToScene, SelectionMenu_Draw, SelectionMenu_Update, SelectionMenu_IsShow)

#endif // SELECTION_MENU_H