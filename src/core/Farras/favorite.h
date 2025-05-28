#ifndef FAVORITE_H
#define FAVORITE_H

#include <stdbool.h>

typedef struct Favorite Favorite;
typedef struct Favorite {
    int id;          // ID musik yang disimpan sebagai favorite
    Favorite *next;  // Pointer ke item berikutnya dalam daftar favorite
} Favorite;

typedef struct FavoriteList {
    Favorite *head; // Pointer ke item pertama dalam daftar favorite
    int count;      // Jumlah item dalam daftar favorite
} FavoriteList;

// Membuat untuk menyimpan daftar favorite
void CreateFavoriteList(FavoriteList *list);

// Inisialisasi daftar favorite dari file favorites.txt
void initFavoriteList(FavoriteList *list);

// Menambahkan ID musik ke daftar favorite dan file
void AddFavorite(FavoriteList *list, int id);

// Menghapus ID musik dari daftar favorite dan file
void RemoveFavorite(FavoriteList *list, int id);

// Mengecek apakah ID musik ada di daftar favorite
bool IsFavorite(FavoriteList *list, int id);

// Membebaskan memori daftar favorite dan menyimpan ke file
void DestroyFavoriteList(FavoriteList *list);


#endif