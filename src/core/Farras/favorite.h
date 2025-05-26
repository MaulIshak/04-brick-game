#ifndef FAVORITE_H
#define FAVORITE_H

#include <stdbool.h>

typedef struct Favorite Favorite;
typedef struct Favorite {
    int id;
    Favorite *next;
} Favorite;

typedef struct FavoriteList {
    Favorite *head;
    int count;
} FavoriteList;

void CreateFavoriteList(FavoriteList *list);
void initFavoriteList(FavoriteList *list);
void AddFavorite(FavoriteList *list, int id);
void RemoveFavorite(FavoriteList *list, int id);
bool IsFavorite(FavoriteList *list, int id);
void DestroyFavoriteList(FavoriteList *list);

#endif