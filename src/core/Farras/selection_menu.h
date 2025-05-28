#pragma once
#include "context.h"
#include "favorite.h"
#include "flying_object.h"
#include "scene.h"
#include <raylib.h>

#ifndef SELECTION_MENU_H
#define SELECTION_MENU_H
#define MAX_MUSIC_SHOWING 30
typedef struct SelectionMenu {
    AppContext *ctx;

    int indexMove;
    float textOffsetX;

    float rotationOffset;
    float targetRotationOffset;

    float selectionYOffset;
    float targetYOffset;

    bool isShow;

    FlyingObjects flying_objects;
    FavoriteList favoriteList;
    bool showFavoriteList;

    Texture2D background;
} SelectionMenu;

// Fungsi untuk menginisialisasi SelectionMenu
void PressToAction(SelectionMenu *self);
// Fungsi untuk preview musik yang dipilih
void PreviewMusic(SelectionMenu *self);
// Fungsi untuk menggambar item pada SelectionMenu
void MusicList_Draw(SelectionMenu *self);
void FavoriteMusicList_Draw(SelectionMenu *self);
void MusicItem_Draw(SelectionMenu *self, Track track, int index, bool isSelected);
// Fungsi untuk menggambar menu bantuan pada SelectionMenu
void SelectionMenuHelper_Draw(SelectionMenu *self);

void InitSelectionMenu(SelectionMenu *selection_menu, AppContext *ctx);
void SelectionMenu_Draw(SelectionMenu *self);
void SelectionMenu_Update(SelectionMenu *self);
bool SelectionMenu_IsShow(SelectionMenu *self);

impl_scene(SelectionMenu *, SelectionMenu_ToScene, SelectionMenu_Draw, SelectionMenu_Update, SelectionMenu_IsShow)

#endif // SELECTION_MENU_H