#include "context.h"
#include "album_cover.h"
#include "raylib.h"

void AlbumCover_Update(AlbumCover* self){

}
void AlbumCover_Draw(AlbumCover* self){
    const int WIDTH = 150;
    const int PADDING = 3;
    Vector2 album_pos = (Vector2){self->ctx->screen_width - WIDTH - 20, 20};
    Rectangle album_rec = {
        album_pos.x - PADDING, 20 - PADDING, WIDTH + (PADDING * 2), WIDTH + (PADDING * 2) 
    };
    // DrawRectangleRec(album_rec, RED);
    DrawRectangleLinesEx(album_rec, 3, BLACK); 
    Texture2D cover = self->ctx->tracks.track[self->ctx->selected_track].cover;
    DrawTextureEx(cover, (Vector2){album_pos.x, album_pos.y}, 0, 0.33333 , WHITE);
}
bool AlbumCover_IsShow(AlbumCover* self){
    return self->ctx->app_state == APP_SELECT;
}
