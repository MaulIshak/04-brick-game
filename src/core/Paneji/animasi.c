#include "animasi.h"

void LoadTextures(Texture2D textures[]) {
    textures[0] = LoadTexture("Arrow-3.png");  
    textures[1] = LoadTexture("Arrow-2.png");  
    textures[2] = LoadTexture("Arrow.png");    
    textures[3] = LoadTexture("Arrow-1.png");  
    textures[4] = LoadTexture("Arrow_YangAdaPutihnya-03.png");  
    textures[5] = LoadTexture("Arrow_YangAdaPutihnya-01.png");  
    textures[6] = LoadTexture("Arrow_YangAdaPutihnya-04.png");  
    textures[7] = LoadTexture("Arrow_YangAdaPutihnya-02.png");  
}

void UnloadTextures(Texture2D textures[]) {
    for (int i = 0; i < 8; i++) {
        UnloadTexture(textures[i]);
    }
}

void InitPositions(Vector2 positions[], Texture2D textures[]) {
    int offsets[] = {0, 100, 200, 300, 150, 250, 50, 175};

    positions[0] = (Vector2){ SCREEN_WIDTH + offsets[0], SCREEN_HEIGHT / 3 };
    positions[1] = (Vector2){ -textures[1].width - offsets[1], 2 * SCREEN_HEIGHT / 3 };
    positions[2] = (Vector2){ SCREEN_WIDTH / 3, SCREEN_HEIGHT + offsets[2] };
    positions[3] = (Vector2){ 2 * SCREEN_WIDTH / 3, -textures[3].height - offsets[3] };
    positions[4] = (Vector2){ -textures[4].width - offsets[4], SCREEN_HEIGHT / 3 };
    positions[5] = (Vector2){ SCREEN_WIDTH + offsets[5], 2 * SCREEN_HEIGHT / 3 };
    positions[6] = (Vector2){ SCREEN_WIDTH / 3, -textures[6].height - offsets[6] };
    positions[7] = (Vector2){ 2 * SCREEN_WIDTH / 3, SCREEN_HEIGHT + offsets[7] };
}

void UpdatePositions(Vector2 positions[], Texture2D textures[]) {
    positions[0].x -= TEXT_SPEED;
    positions[1].x += TEXT_SPEED;
    positions[2].y -= TEXT_SPEED;
    positions[3].y += TEXT_SPEED;

    positions[4].x += TEXT_SPEED;
    positions[5].x -= TEXT_SPEED;
    positions[6].y += TEXT_SPEED;
    positions[7].y -= TEXT_SPEED;

    if (positions[0].x < -textures[0].width) positions[0].x = SCREEN_WIDTH;
    if (positions[1].x > SCREEN_WIDTH) positions[1].x = -textures[1].width;
    if (positions[2].y < -textures[2].height) positions[2].y = SCREEN_HEIGHT;
    if (positions[3].y > SCREEN_HEIGHT) positions[3].y = -textures[3].height;

    if (positions[4].x > SCREEN_WIDTH) positions[4].x = -textures[4].width;
    if (positions[5].x < -textures[5].width) positions[5].x = SCREEN_WIDTH;
    if (positions[6].y > SCREEN_HEIGHT) positions[6].y = -textures[6].height;
    if (positions[7].y < -textures[7].height) positions[7].y = SCREEN_HEIGHT;
}

void DrawTextures(Texture2D textures[], Vector2 positions[]) {
    for (int i = 0; i < 8; i++) {
        DrawTextureEx(textures[i], positions[i], 0.0f, 1.5f, WHITE);
    }
}

