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
} SelectionMenu;


void SelectionMenu_Draw(SelectionMenu *self);
void SelectionMenu_Update(SelectionMenu *self);
bool SelectionMenu_IsShow(SelectionMenu *self);


impl_scene(SelectionMenu*, SelectionMenu_ToScene, SelectionMenu_Draw, SelectionMenu_Update, SelectionMenu_IsShow)

#endif // SELECTION_MENU_H