/*
 * Modul: Manajemen Lagu Favorit
 * 
 * Modul ini menangani penyimpanan dan pengelolaan daftar lagu favorit.
 * Fitur utama:
 * - Penyimpanan daftar favorit
 * - Penambahan dan penghapusan lagu favorit
 * - Penyimpanan permanen menggunakan file
 * - Pengecekan status favorit
 */

#ifndef FAVORITE_H
#define FAVORITE_H

#include <stdbool.h>
#include "raylib.h"
/**
 * Struktur untuk menyimpan satu lagu favorit
 */
typedef struct Favorite Favorite;
typedef struct Favorite {
    int id;          // ID lagu favorit
    Favorite *next;
} Favorite;


/**
 * Struktur untuk mengelola daftar favorit
 */
typedef struct FavoriteList {
    Favorite *head;  // Pointer ke lagu pertama
    int count;       // Jumlah lagu favorit
    int selectedIndex; // Indeks lagu favorit yang dipilih
    Texture2D favoriteIcon; // Ikon favorit untuk ditampilkan
    Texture2D notFavoriteIcon; // Ikon tidak favorit untuk ditampilkan
} FavoriteList;

/**
 * Membuat daftar favorit baru
 */
void CreateFavoriteList(FavoriteList *list);

/**
 * Memuat daftar favorit dari file
 */
void initFavoriteList(FavoriteList *list);

/**
 * Menambah lagu ke favorit
 */
void AddFavorite(FavoriteList *list, int id);

/**
 * Menghapus lagu dari favorit
 */
void RemoveFavorite(FavoriteList *list, int id);

/**
 * Mengecek status favorit lagu
 */
bool IsFavorite(FavoriteList *list, int id);

/**
 * Mengambil lagu favorit berdasarkan indeks
 */
Favorite getFavoriteByFavoriteIndex(FavoriteList *list, int index);

/**
 * Membersihkan daftar favorit
 */
void DestroyFavoriteList(FavoriteList *list);

#endif