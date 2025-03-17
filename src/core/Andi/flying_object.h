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
} FlyingObjects;


FlyingObjects FlyingObject_Create();
void FlyingObject_Update(FlyingObjects *self, AppContext *ctx) ;
void FlyingObject_Draw(FlyingObjects *self);

FlyingObject ObjectCreate(int widht, int height);
Vector2 FlyingObjectRandomPos(int height);
Vector2 FlyingObjectRandomTarget(int width, int height);

#endif