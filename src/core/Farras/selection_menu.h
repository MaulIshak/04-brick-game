/*
 * Modul: Menu Pemilihan Lagu
 * 
 * Modul ini mengimplementasikan antarmuka pemilihan lagu dalam permainan.
 * Fitur utama:
 * - Tampilan roda berputar untuk daftar lagu
 * - Preview musik sebelum bermain
 * - Manajemen lagu favorit
 * - Animasi transisi halus
 * - Efek visual latar belakang
 */

#pragma once
#include "context.h"
#include "favorite.h"
#include "flying_object.h"
#include "scene.h"
#include <raylib.h>

#ifndef SELECTION_MENU_H
#define SELECTION_MENU_H

/**
 * Jumlah maksimum lagu yang ditampilkan dalam roda musik
 */
#define MAX_MUSIC_SHOWING 30

/**
 * Struktur utama menu pemilihan lagu
 */
typedef struct SelectionMenu {
    AppContext *ctx;              // Konteks aplikasi
    int indexMove;               // Offset indeks untuk pergerakan roda
    float textOffsetX;           // Offset untuk scrolling teks panjang
    float rotationOffset;        // Offset rotasi saat ini
    float targetRotationOffset;  // Target rotasi untuk animasi
    float selectionYOffset;      // Offset Y saat ini
    float targetYOffset;         // Target offset Y untuk animasi
    bool isShow;                 // Status tampilan menu
    FlyingObjects flying_objects; // Objek latar belakang bergerak
    FavoriteList favoriteList;   // Daftar lagu favorit
    bool showFavoriteList;       // Status tampilan daftar favorit
    Texture2D background;        // Tekstur latar belakang
} SelectionMenu;

/**
 * Menangani input pengguna untuk navigasi menu utama
 */
void PressToAction(SelectionMenu *self);

/**
 * Menangani input pengguna khusus untuk daftar favorit
 */
void PressToActionFavorite(SelectionMenu *self);

/**
 * Mengelola preview musik yang sedang dipilih
 */
void PreviewMusic(SelectionMenu *self);

/**
 * Menggambar daftar lagu dalam mode normal
 */
void MusicList_Draw(SelectionMenu *self);

/**
 * Menggambar daftar lagu dalam mode favorit
 */
void FavoriteMusicList_Draw(SelectionMenu *self);

/**
 * Menggambar satu item lagu dengan efek visual
 */
void MusicItem_Draw(SelectionMenu *self, Track track, int index, bool isSelected);

/**
 * Menggambar panduan kontrol dan tombol
 */
void SelectionMenuHelper_Draw(SelectionMenu *self);

/**
 * Inisialisasi menu pemilihan lagu
 */
void InitSelectionMenu(SelectionMenu *selection_menu, AppContext *ctx);

/**
 * Fungsi render utama menu
 */
void SelectionMenu_Draw(SelectionMenu *self);

/**
 * Update status dan animasi menu
 */
void SelectionMenu_Update(SelectionMenu *self);

/**
 * Memeriksa apakah menu harus ditampilkan
 */
bool SelectionMenu_IsShow(SelectionMenu *self);

// Implementasi scene untuk menu pemilihan
impl_scene(SelectionMenu *, SelectionMenu_ToScene, SelectionMenu_Draw, SelectionMenu_Update, SelectionMenu_IsShow)

#endif // SELECTION_MENU_H