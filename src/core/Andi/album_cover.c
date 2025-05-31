#include "context.h"
#include "album_cover.h"
#include "raylib.h"
#include "raymath.h"

AlbumCover AlbumCover_Create(AppContext *ctx) {
    AlbumCover cover;
    cover.ctx = ctx;
    cover.CDTexture = LoadTexture("resources/texture/CD.png");
    cover.CDRotation = 0;
    return cover;
}

void AlbumCover_Destroy(AlbumCover* self) {
    UnloadTexture(self->CDTexture);
 }

void AlbumCover_Update(AlbumCover* self){
    self->CDRotation += 0.2;
}

void AlbumCover_Draw(AlbumCover* self){
    const int WIDTH = 150;
    const int PADDING = 3;
    const float CD_SCALE = 0.14;
    const float COVER_SCALE = 0.33333;
    Vector2 album_pos = (Vector2){self->ctx->screen_width - WIDTH - 20, 20};
    Rectangle album_rec = {
        album_pos.x - PADDING, 20 - PADDING, WIDTH + (PADDING * 2), WIDTH + (PADDING * 2) 
    };
    // DrawRectangleRec(album_rec, RED);
    Texture2D cover = GetSelectedTrack(self->ctx).cover;
    Vector2 CDOrigin = {(self->CDTexture.width * CD_SCALE) / 2, (self->CDTexture.height * CD_SCALE) / 2 };

    Rectangle CDLocSrc = {0,0, self->CDTexture.width, self->CDTexture.height};
    Rectangle CDDestSrc = {album_pos.x, album_pos.y + (cover.height * COVER_SCALE / 2 ), self->CDTexture.width * CD_SCALE, self->CDTexture.height * CD_SCALE};
    DrawTexturePro(self->CDTexture, CDLocSrc, CDDestSrc, CDOrigin, self->CDRotation, WHITE);
    // DrawTextureEx(self->CDTexture, , self->CDRotation, CD_SCALE, WHITE);
    DrawRectangleLinesEx(album_rec, 3, BLACK); 
    DrawTextureEx(cover, (Vector2){album_pos.x, album_pos.y}, 0,COVER_SCALE, WHITE);
}
bool AlbumCover_IsShow(AlbumCover* self){
    return self->ctx->app_state == APP_SELECT;
}
