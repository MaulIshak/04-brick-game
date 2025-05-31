/*
 * Modul: Pengelolaan Musik Favorit
 * 
 * Modul ini menyediakan fungsionalitas untuk mengelola daftar lagu favorit dalam permainan.
 * Mengimplementasikan struktur data linked list untuk menyimpan ID lagu dan menyediakan
 * penyimpanan permanen melalui file teks (favorites.txt). Modul ini mendukung penambahan,
 * penghapusan, dan pengecekan status favorit dari lagu-lagu.
 */

#ifndef FAVORITE_H
#define FAVORITE_H

#include <stdbool.h>

/**
 * Merepresentasikan satu lagu favorit dalam linked list.
 */
typedef struct Favorite Favorite;
typedef struct Favorite {
    int id;          // ID lagu yang disimpan
    Favorite *next;  // Pointer ke lagu favorit berikutnya dalam list
} Favorite;

/**
 * Wadah untuk menyimpan daftar lagu favorit.
 */
typedef struct FavoriteList {
    Favorite *head;  // Pointer ke lagu favorit pertama dalam list
    int count;       // Jumlah total lagu dalam daftar favorit
} FavoriteList;

/**
 * Membuat daftar favorit kosong.
 * @param list Pointer ke struktur FavoriteList yang akan diinisialisasi
 */
void CreateFavoriteList(FavoriteList *list);

/**
 * Memuat daftar lagu favorit dari file favorites.txt.
 * Membuat file baru jika belum ada.
 * @param list Pointer ke FavoriteList yang akan diisi
 */
void initFavoriteList(FavoriteList *list);

/**
 * Menambahkan lagu ke daftar favorit dan menyimpannya ke file.
 * Mencegah duplikasi entri.
 * @param list Pointer ke FavoriteList
 * @param id ID lagu yang akan ditambahkan
 */
void AddFavorite(FavoriteList *list, int id);

/**
 * Menghapus lagu dari daftar favorit dan memperbarui file.
 * @param list Pointer ke FavoriteList
 * @param id ID lagu yang akan dihapus
 */
void RemoveFavorite(FavoriteList *list, int id);

/**
 * Memeriksa apakah sebuah lagu ada dalam daftar favorit.
 * @param list Pointer ke FavoriteList
 * @param id ID lagu yang akan diperiksa
 * @return true jika lagu ada dalam favorit, false jika tidak
 */
bool IsFavorite(FavoriteList *list, int id);

/**
 * Membebaskan memori yang digunakan daftar favorit dan menyimpan ke file.
 * @param list Pointer ke FavoriteList yang akan dihapus
 */
void DestroyFavoriteList(FavoriteList *list);

#endif