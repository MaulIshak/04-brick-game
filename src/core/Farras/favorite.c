/*
 * Implementation of the Favorite Music Management module.
 * This file implements the persistence and management of favorite music tracks
 * using a singly linked list and file I/O operations.
 */

#include "favorite.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Initialize an empty favorite list structure.
 */
void CreateFavoriteList(FavoriteList *list) {
    list->head = NULL;
    list->count = 0;
}

/**
 * Load favorite tracks from the favorites.txt file.
 * If the file doesn't exist, creates a new empty file.
 * Each line in the file contains one music track ID.
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
 * Add a new music track to favorites if not already present.
 * Implements a prepend operation (O(1)) to the linked list.
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
 * Remove a music track from favorites if present.
 * Implements a linear search (O(n)) through the linked list.
 * Updates list structure and frees memory.
 */
void RemoveFavorite(FavoriteList *list, int id) {
    Favorite *current = list->head;
    Favorite *previous = NULL;

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

        previous = current;
        current = current->next;
    }
}

/**
 * Check if a music track is marked as favorite.
 * Implements a linear search (O(n)) through the linked list.
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

/**
 * Clean up the favorites list and save current state to file.
 * Frees all allocated memory and writes IDs to favorites.txt.
 * Creates the file if it doesn't exist.
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
