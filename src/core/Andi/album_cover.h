#ifndef ALBUM_COVER_H
#define ALBUM_COVER_H

#include "context.h"
#include "scene.h"

typedef struct AlbumCover {
    AppContext *ctx;
    Texture2D CDTexture;
    float CDRotation;
} AlbumCover;

AlbumCover AlbumCover_Create(AppContext *ctx);
void AlbumCover_Destroy(AlbumCover* self);
void AlbumCover_Update(AlbumCover* self);
void AlbumCover_Draw(AlbumCover* self);
bool AlbumCover_IsShow(AlbumCover* self);


impl_scene(AlbumCover*, AlbumCover_ToScene, AlbumCover_Draw, AlbumCover_Update, AlbumCover_IsShow)

#endif // ALBUM_COVER_H