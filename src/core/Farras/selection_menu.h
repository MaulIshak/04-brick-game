/*
 * Modul: Menu Pemilihan Musik
 * 
 * Modul ini mengimplementasikan antarmuka pemilihan musik dalam permainan dimana pemain dapat:
 * - Menjelajahi lagu-lagu yang tersedia dalam tampilan roda berputar
 * - Mendengarkan preview lagu sebelum bermain
 * - Menambah/menghapus lagu ke/dari daftar favorit
 * - Beralih antara tampilan daftar normal dan daftar favorit
 * - Memulai permainan dengan lagu yang dipilih
 */

#pragma once
#include <raylib.h>
#include "scene.h"
#include "context.h"
#include "flying_object.h"

#ifndef SELECTION_MENU_H
#define SELECTION_MENU_H

/**
 * Jumlah maksimum lagu yang ditampilkan dalam tampilan roda berputar
 */
#define MAX_MUSIC_SHOWING 30

/**
 * Struktur utama untuk menu pemilihan musik
 */
typedef struct SelectionMenu {
    AppContext *ctx;              // Konteks aplikasi dengan status permainan
    float rotationOffset;         // Rotasi saat ini dari roda musik
    float targetRotationOffset;   // Target rotasi untuk animasi halus
    float selectionYOffset;       // Offset Y saat ini untuk seleksi
    float targetYOffset;          // Target offset Y untuk animasi halus
    bool isShow;                  // Apakah menu sedang ditampilkan
    FlyingObjects flying_objects; // Efek visual latar belakang
    Texture2D background;         // Tekstur latar belakang menu
} SelectionMenu;

/**
 * Menangani input keyboard untuk navigasi dan pemilihan menu
 */
void PressToAction(SelectionMenu *self);

/**
 * Mengelola fungsi preview musik saat menjelajahi lagu
 */
void PreviewMusic(SelectionMenu *self);

/**
 * Menggambar item menu individual dalam roda berputar
 */
void SelectionMenuItem_Draw(SelectionMenu *self);

/**
 * Menggambar elemen pembantu UI seperti petunjuk tombol
 */
void SelectionMenuHelper_Draw(SelectionMenu *self);

/**
 * Fungsi penggambaran utama untuk menu pemilihan
 */
void SelectionMenu_Draw(SelectionMenu *self);

/**
 * Memperbarui status menu, animasi dan preview musik
 */
void SelectionMenu_Update(SelectionMenu *self);

/**
 * Memeriksa apakah menu pemilihan harus ditampilkan
 * @return true jika menu harus ditampilkan, false jika tidak
 */
bool SelectionMenu_IsShow(SelectionMenu *self);

/**
 * Menginisialisasi instance menu pemilihan baru
 */
void InitSelectionMenu(SelectionMenu *selection_menu, AppContext *ctx);

// Implementasi scene untuk menu pemilihan
impl_scene(SelectionMenu*, SelectionMenu_ToScene, SelectionMenu_Draw, SelectionMenu_Update, SelectionMenu_IsShow)

#endif // SELECTION_MENU_H