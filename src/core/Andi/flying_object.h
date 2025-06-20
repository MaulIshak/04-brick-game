/**
 * flying_object.h
 * 
 * pembuat: andiputraw
 * 
 * Modul-modul yang tersedia di file ini berfungsi untuk membuat object terbang di belakang layar.
 * di dalam game, ini terlihat di saat pemilihan musik.
 */
#ifndef FLYING_OBJECT_H
#define FLYING_OBJECT_H

#include "raylib.h"
#include "context.h"
#define FLYING_OBJECT_COUNT 5
#define FLYING_OBJECT_SPEED 0.5

typedef enum FlyingObjectType {
    OBJECT_TRIANGLE,
    OBJECT_CIRCLE,
    OBJECT_AJAW,
    OBJECT_CITLALIN,
} FlyingObjectType;

typedef struct FlyingObject {
    Vector2 pos;
    Vector2 target;
    float rotation;
    bool alive;
    float alpha;
    FlyingObjectType type;
} FlyingObject;

typedef struct FlyingObjects {
    FlyingObject objs[FLYING_OBJECT_COUNT];
    Texture2D AjawTexture;
    Texture2D CitlalinTexture;
    Texture2D CDTexture;
    AppContext *ctx;
} FlyingObjects;

// Membuat flying objects
FlyingObjects FlyingObject_Create(AppContext *ctx);
// Mengupdate flying object
void FlyingObject_Update(FlyingObjects *self, AppContext *ctx) ;
// Menggabar flying object
void FlyingObject_Draw(FlyingObjects *self);

// Membuat object
FlyingObject ObjectCreate(int widht, int height);
// Set flying object di posisi random dengan ketinggian tertentu
Vector2 FlyingObjectRandomPos(int height);
// Set target pergerakan dari flying object
Vector2 FlyingObjectRandomTarget(int width, int height);

#endif