/*
 * Modul: Manajemen Lagu Favorit
 *
 * Implementasi dari modul pengelolaan daftar lagu favorit.
 * Menggunakan linked list dan operasi file untuk menyimpan
 * daftar lagu favorit secara permanen.
 */

#include "favorite.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Membuat daftar favorit baru
 */
void CreateFavoriteList(FavoriteList *list) {
    list->head = NULL;
    list->count = 0;
    list->selectedIndex = 0;
    list->favoriteIcon = LoadTexture("resources/texture/Heart.png");
    list->notFavoriteIcon = LoadTexture("resources/texture/Heart_Gray.png");
}

/**
 * Memuat daftar favorit dari file
 */
void initFavoriteList(FavoriteList *list) {
    FILE *file = fopen("favorites.txt", "r");

    if (file == NULL) {
        printf("favorites.txt not found. Creating a new one...\n");

        file = fopen("favorites.txt", "w");
        if (file == NULL) {
            fprintf(stderr, "Failed to create favorites.txt.\n");
            return;
        }

        printf("favorites.txt created successfully.\n");
        fclose(file);
        return;
    }

    printf("Load favorites from favorites.txt...\n");
    int id;
    while (fscanf(file, "%d", &id) == 1) {
        AddFavorite(list, id);
    }

    fclose(file);
}

/**
 * Menambah lagu ke favorit
 */
void AddFavorite(FavoriteList *list, int id) {
    if (IsFavorite(list, id))
        return;

    Favorite *new_favorite = malloc(sizeof(Favorite));
    if (new_favorite == NULL) {
        fprintf(stderr, "Failed to allocate memory for new favorite.\n");
        return;
    }

    new_favorite->id = id;
    new_favorite->next = list->head;
    list->head = new_favorite;
    list->count++;
}

/**
 * Menghapus lagu dari favorit
 */
void RemoveFavorite(FavoriteList *list, int id) {
    Favorite *current = list->head;
    Favorite *previous = NULL;

    int count = 0;
    while (current != NULL) {
        if (current->id == id) {
            if (previous == NULL) {
                list->head = current->next;
            } else {
                previous->next = current->next;
            }

            free(current);
            list->count--;
            break;
        }
        count++;
        previous = current;
        current = current->next;
    }
}

/**
 * Mengecek status favorit lagu
 */
bool IsFavorite(FavoriteList *list, int id) {
    Favorite *current = list->head;
    while (current != NULL) {
        if (current->id == id) {
            return true;
        }
        current = current->next;
    }
    return false;
}

Favorite getFavoriteByFavoriteIndex(FavoriteList *list, int index) {
    if (index < 0 || index >= list->count) {
        fprintf(stderr, "Index out of bounds for favorite list.\n");
        return (Favorite){0};
    }

    Favorite *current = list->head;
    int i;
    for (i = 0; i < index && current != NULL; i++) {
        current = current->next;
    }

    return *current;
}

/**
 * Membersihkan daftar favorit
 */
void DestroyFavoriteList(FavoriteList *list) {
    Favorite *current = list->head;

    FILE *file = fopen("favorites.txt", "w");
    if (file == NULL) {
        printf("favorites.txt not found. Creating a new one...\n");

        file = fopen("favorites.txt", "w");
        if (file == NULL) {
            fprintf(stderr, "Failed to create favorites.txt.\n");
            return;
        }

        printf("favorites.txt created successfully.\n");
        fclose(file);
        return;
    }

    while (current != NULL) {
        Favorite *next = current->next;
        fprintf(file, "%d\n", current->id);

        free(current);
        current = next;
    }

    fclose(file);

    list->head = NULL;
    list->count = 0;
}
