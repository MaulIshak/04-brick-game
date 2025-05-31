/**
 * album_cover.h
 * 
 * pembuat: andiputraw
 * 
 * Modul-modul di file ini digunakan pada saat menggambar album cover di selection menu
 */
#ifndef ALBUM_COVER_H
#define ALBUM_COVER_H

#include "context.h"
#include "scene.h"

typedef struct AlbumCover {
    AppContext *ctx;
    Texture2D CDTexture;
    float CDRotation;
} AlbumCover;

// Membuat album cover
AlbumCover AlbumCover_Create(AppContext *ctx);
// Mendeinisialisasi struct album cover
void AlbumCover_Destroy(AlbumCover* self);
// Mengupdate struct album cover
void AlbumCover_Update(AlbumCover* self);
// Menggambar struct album cover
void AlbumCover_Draw(AlbumCover* self);
// Pengecekan jika album cover perlu di gambar atau tidak
bool AlbumCover_IsShow(AlbumCover* self);


impl_scene(AlbumCover*, AlbumCover_ToScene, AlbumCover_Draw, AlbumCover_Update, AlbumCover_IsShow)

#endif // ALBUM_COVER_H