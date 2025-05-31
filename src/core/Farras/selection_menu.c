/*
 * Implementasi Menu Pemilihan Lagu
 *
 * File ini berisi implementasi antarmuka pemilihan lagu dengan
 * tampilan roda berputar dan manajemen lagu favorit.
 */

#include "selection_menu.h"
#include "context.h"
#include "favorite.h"
#include "flying_object.h"
#include "math.h"
#include "scene.h"
#include <stdio.h>
/**
 * Inisialisasi menu pemilihan
 */
void InitSelectionMenu(SelectionMenu *self, AppContext *ctx) {
    CreateFavoriteList(&self->favoriteList);
    initFavoriteList(&self->favoriteList);

    self->ctx = ctx;

    self->indexMove = 0;
    self->textOffsetX = 0;

    self->rotationOffset = 0;
    self->targetRotationOffset = 0;

    self->selectionYOffset = 0;
    self->targetYOffset = 0;

    self->isShow = false;
    self->showFavoriteList = false;
    self->seletedFavoriteTrack = 0;

    self->flying_objects = FlyingObject_Create(ctx);
}

/**
 * Update status menu dan animasi
 */
void SelectionMenu_Update(SelectionMenu *self) {
    FlyingObject_Update(&self->flying_objects, self->ctx);

    if (IsKeyPressed(KEY_ENTER)) {
        StopSelectedTrack(self->ctx);
        SeekMusicStream(GetSelectedTrack(self->ctx).music, 0.1f);
        self->ctx->app_state = APP_PLAYING;
    }

    if (IsKeyPressed(KEY_F)) {
        Track selectedTrack = GetSelectedTrack(self->ctx);

        if (IsFavorite(&self->favoriteList, selectedTrack.music_id)) {
            RemoveFavorite(&self->favoriteList, selectedTrack.music_id);

            if (self->showFavoriteList && self->favoriteList.count > 0) {
                if (self->favoriteList.selectedIndex >= self->favoriteList.count) {
                    self->favoriteList.selectedIndex = self->favoriteList.count - 1;
                } else {
                    self->favoriteList.selectedIndex =
                        (self->favoriteList.selectedIndex + 1) % self->favoriteList.count;
                }

                Favorite fav = getFavoriteByFavoriteIndex(&self->favoriteList, self->favoriteList.selectedIndex);
                self->ctx->selected_track = fav.id - 1;
            }
        } else {
            AddFavorite(&self->favoriteList, selectedTrack.music_id);
        }
    }

    if (IsKeyPressed(KEY_J)) {
        self->showFavoriteList = !self->showFavoriteList;
        self->ctx->selected_track = 0;
    }

    if (self->showFavoriteList) {
        PressToActionFavorite(self);
    } else {
        PressToAction(self);
    }

    PreviewMusic(self);

    if (self->showFavoriteList) {
        self->targetYOffset = -self->favoriteList.selectedIndex * 110;
        self->selectionYOffset += (self->targetYOffset - self->selectionYOffset) * 0.1f;

        self->targetRotationOffset = -self->favoriteList.selectedIndex;
        self->rotationOffset += (self->targetRotationOffset - self->rotationOffset) * 0.1f;
    } else {
        self->targetYOffset = -self->ctx->selected_track * 110;
        self->selectionYOffset += (self->targetYOffset - self->selectionYOffset) * 0.1f;

        self->targetRotationOffset = -self->ctx->selected_track;
        self->rotationOffset += (self->targetRotationOffset - self->rotationOffset) * 0.1f;
    }
}

/**
 * Menggambar tampilan menu
 */
void SelectionMenu_Draw(SelectionMenu *self) {
    ClearBackground(WHITE);

    DrawCircleGradient(self->ctx->screen_width / 2, self->ctx->screen_height / 2, self->ctx->screen_height, (Color){254, 250, 148, 255}, (Color){255, 147, 98, 255});
    DrawCircleGradient(self->ctx->screen_width / 2 - 100, self->ctx->screen_height / 2 + 500, self->ctx->screen_height, (Color){255, 147, 98, 255}, (Color){255, 92, 93, 255});
    DrawCircleGradient(self->ctx->screen_width / 2 - 200, self->ctx->screen_height / 2 + 600, self->ctx->screen_height, (Color){255, 92, 93, 255}, (Color){128, 69, 255, 255});
    DrawCircleGradient(self->ctx->screen_width / 2 - 300, self->ctx->screen_height / 2 + 700, self->ctx->screen_height, (Color){128, 69, 255, 255}, (Color){57, 43, 214, 255});
    DrawCircleGradient(self->ctx->screen_width / 2 - 400, self->ctx->screen_height / 2 + 800, self->ctx->screen_height, (Color){57, 43, 214, 255}, (Color){24, 29, 149, 255});
    DrawCircle(self->ctx->screen_width / 2 - 500, self->ctx->screen_height / 2 + 900, self->ctx->screen_height, (Color){24, 29, 149, 255});
    FlyingObject_Draw(&self->flying_objects);

    SelectionMenuHelper_Draw(self);

    if (self->showFavoriteList) {
        FavoriteMusicList_Draw(self);
    } else {
        MusicList_Draw(self);
    }
}

/**
 * Menampilkan daftar lagu utama
 */
void MusicList_Draw(SelectionMenu *self) {
    int selected = self->ctx->selected_track;
    for (int i = 0 + self->indexMove; i < self->ctx->tracks.len + self->indexMove; i++) {
        Track current = GetTrack(self->ctx->tracks, i);

        bool isSelected = (i == selected);

        MusicItem_Draw(self, current, i, isSelected);
    }
}

/**
 * Menampilkan daftar lagu favorit
 */
void FavoriteMusicList_Draw(SelectionMenu *self) {
    Favorite *current = self->favoriteList.head;
    int i = 0;
    while (current != NULL) {
        Track track = GetTrack(self->ctx->tracks, current->id - 1);

        bool isSelected = (self->favoriteList.selectedIndex == i);

        MusicItem_Draw(self, track, i, isSelected);

        current = current->next;
        i++;
    }
}

/**
 * Menggambar item lagu
 */
void MusicItem_Draw(SelectionMenu *self, Track track, int index, bool isSelected) {
    const int boxWidth = 400;
    const float fontsize = 30;
    const float textPadding = 10;

    int selected = self->showFavoriteList ? self->favoriteList.selectedIndex : self->ctx->selected_track;
    int distance = abs(index - selected);

    float scale = isSelected ? 1.2f : (1.2f / (1.0f + 0.2f * distance));
    Color textColor = isSelected ? GOLD : LIGHTGRAY;
    Color bgColor = isSelected ? RAYWHITE : Fade(GRAY, 0.8f);

    float r = self->ctx->screen_height / 2;
    float angle = ((index + self->rotationOffset) * (PI * 2)) / MAX_MUSIC_SHOWING;

    float y = self->ctx->screen_height / 2 + r * sin(angle);
    float x = self->ctx->screen_width / 2 - self->ctx->screen_width + r * cos(angle);

    Vector2 textSize = MeasureTextEx(self->ctx->font, track.music_name, fontsize * scale, 2);

    // Background
    DrawRectangleGradientH(x - textPadding, y - textSize.y + textPadding / 2, boxWidth + textPadding, textSize.y * 2,
                           bgColor, isSelected ? Fade(bgColor, 1.0f) : Fade(bgColor, 0.0f));

    // Scrolling text
    if (isSelected && textSize.x > boxWidth) {
        self->textOffsetX += 50 * GetFrameTime();
        if (self->textOffsetX > textSize.x) {
            self->textOffsetX = -boxWidth;
        }
    }

    // Draw ranking A, B, C, D, E
    char grade;
    Color gradeColor = WHITE;
    float accuracy = track.accuracy;
    if (accuracy >= 90) {
        grade = 'S';
        gradeColor = GOLD;
    } else if (accuracy >= 80) {
        grade = 'A';
        gradeColor = GREEN;
    } else if (accuracy >= 70) {
        grade = 'B';
        gradeColor = BLUE;
    } else if (accuracy >= 60) {
        grade = 'C';
        gradeColor = ORANGE;
    } else {
        grade = 'D';
        gradeColor = RED;
    }

    DrawTextEx(self->ctx->font, TextFormat("%c", grade),
               (Vector2){x + 10, y - 65 * scale}, 100 * scale, 2, gradeColor);

    BeginScissorMode(x + 75, y - textSize.y + textPadding / 2, boxWidth - 65 - 50 - textPadding, textSize.y + textPadding);

    // Music name
    DrawTextEx(self->ctx->font, track.music_name,
               (Vector2){x + 75 - (textSize.x > boxWidth ? self->textOffsetX : 0), y - textSize.y + textPadding},
               fontsize * scale, 2, textColor);

    EndScissorMode();

    // "Favorite" label
    if (IsFavorite(&self->favoriteList, track.music_id)) {
        DrawTexture(self->favoriteList.favoriteIcon, x + boxWidth - 40, y - textSize.y + textPadding, Fade(WHITE, scale));
    } else {
        DrawTexture(self->favoriteList.notFavoriteIcon, x + boxWidth - 40, y - textSize.y + textPadding, Fade(WHITE, scale));
    }

    // Score
    DrawTextEx(self->ctx->font, TextFormat("Best Score: %d", track.high_score),
               (Vector2){x + 75, y + textPadding / 2}, (fontsize - 10) * scale, 2, textColor);
}

/**
 * Fungsi helper untuk menggambar tombol
 * Membuat tombol dengan border dan latar belakang
 */
void buttonDraw(Font font, Vector2 position, char *text, float fontSize, float spacing, Color textColor, Color bgColor, Color borderColor) {
    Vector2 textVector = MeasureTextEx(font, text, fontSize, spacing);

    float xPosition = position.x - textVector.x / 2;
    float yPosition = position.y - textVector.y / 2;

    DrawRectangleRoundedLinesEx((Rectangle){xPosition - 5, yPosition, textVector.x + 10, textVector.y}, 0.4f, 1, 3.5f, borderColor);
    DrawRectangleRounded((Rectangle){xPosition - 5, yPosition, textVector.x + 10, textVector.y}, 0.3f, 1, bgColor);
    DrawTextEx(font, text, (Vector2){xPosition, yPosition}, fontSize, spacing, textColor);
}

/**
 * Menampilkan panduan kontrol
 */
void SelectionMenuHelper_Draw(SelectionMenu *self) {
    Font font = self->ctx->font;
    float screenWidth = self->ctx->screen_width;
    float screenHeight = self->ctx->screen_height;

    screenHeight -= 30;
    buttonDraw(font, (Vector2){screenWidth - 75, screenHeight}, "     D/K     ", 22, 2, WHITE, DARKGRAY, GRAY);
    buttonDraw(font, (Vector2){screenWidth - 180, screenHeight}, "(Select)", 18, 1, WHITE, BLANK, BLANK);

    screenHeight -= 30;
    buttonDraw(font, (Vector2){screenWidth - 50, screenHeight}, "   F   ", 22, 2, WHITE, RED, WHITE);
    buttonDraw(font, (Vector2){screenWidth - 150, screenHeight}, "(Favorite Music)", 18, 1, WHITE, BLANK, BLANK);

    screenHeight -= 30;
    buttonDraw(font, (Vector2){screenWidth - 50, screenHeight}, "   J   ", 22, 2, WHITE, PINK, WHITE);
    buttonDraw(font, (Vector2){screenWidth - 165, screenHeight}, "(List Favorite Music)", 18, 1, WHITE, BLANK, BLANK);

    screenHeight -= 30;
    buttonDraw(font, (Vector2){screenWidth - 50, screenHeight}, "Enter", 22, 2, ORANGE, YELLOW, GOLD);
    buttonDraw(font, (Vector2){screenWidth - 110, screenHeight}, "(Play)", 18, 1, WHITE, BLANK, BLANK);
}

/**
 * Mengelola preview musik
 */
void PreviewMusic(SelectionMenu *self) {
    if (self->ctx->app_state != APP_SELECT)
        return;

    static bool isCanPlay = false;
    static float time = 0.0f;
    static float volume = 0.0f;
    if (IsSelectedMusicEnd(self->ctx)) {
        time += GetFrameTime();
        if (time > 1.5f) {
            isCanPlay = true;
            time = 0.0f;
        }
    }

    if (!self->ctx->is_music_playing || isCanPlay) {
        isCanPlay = false;
        volume = 0.0f;
        PlaySelectedTrack(self->ctx);
        SeekMusicStream(GetSelectedTrack(self->ctx).music, 10);
    }

    if (self->ctx->is_music_playing && volume < 1.0f) {
        volume += 0.01f;
        SetMusicVolume(GetSelectedTrack(self->ctx).music, volume);
    }
}

/**
 * Menangani input di mode favorit
 */
void PressToActionFavorite(SelectionMenu *self) {
    static float time = 0;
    static float delay = 0.35f;
    static bool keyHeld = false;

    float maxDelay = 0.01f;
    float minDelay = 0.35f;
    float delayReduction = 0.0075f;
    float delayRecovery = 0.0025f;
    float frameThreshold = 0.1f;

    int favoriteCount = self->favoriteList.count;
    if (favoriteCount == 0)
        return;

    if (IsKeyPressed(KEY_K)) {
        self->favoriteList.selectedIndex = (self->favoriteList.selectedIndex + 1) % favoriteCount;
        self->ctx->selected_track = getFavoriteByFavoriteIndex(&self->favoriteList, self->favoriteList.selectedIndex).id - 1;
        keyHeld = true;
        time = 0;
        self->textOffsetX = 0;
        StopSelectedTrack(self->ctx);
    } else if (IsKeyPressed(KEY_D)) {
        self->favoriteList.selectedIndex = (self->favoriteList.selectedIndex - 1 + favoriteCount) % favoriteCount;
        self->ctx->selected_track = getFavoriteByFavoriteIndex(&self->favoriteList, self->favoriteList.selectedIndex).id - 1;
        keyHeld = true;
        time = 0;
        self->textOffsetX = 0;
        StopSelectedTrack(self->ctx);
    }

    if (IsKeyDown(KEY_K) || IsKeyDown(KEY_D)) {
        self->textOffsetX = 0;
        time += GetFrameTime();

        if (time >= delay) {
            if (!keyHeld || time >= frameThreshold) {
                if (IsKeyDown(KEY_K)) {
                    self->favoriteList.selectedIndex = (self->favoriteList.selectedIndex + 1) % favoriteCount;
                } else if (IsKeyDown(KEY_D)) {
                    self->favoriteList.selectedIndex = (self->favoriteList.selectedIndex - 1 + favoriteCount) % favoriteCount;
                }
                self->ctx->selected_track = getFavoriteByFavoriteIndex(&self->favoriteList, self->favoriteList.selectedIndex).id - 1;
                time = 0;
                keyHeld = true;
            }
        }

        if (delay > maxDelay)
            delay -= delayReduction;
    } else {
        if (delay < minDelay)
            delay += delayRecovery;
        keyHeld = false;
    }
}

/**
 * Menangani input di mode utama
 */
void PressToAction(SelectionMenu *self) {
    static float time = 0;
    static float delay = 0.35f;
    static bool keyHeld = false;

    float maxDelay = 0.01f;
    float minDelay = 0.35f;
    float delayReduction = 0.0075f;
    float delayRecovery = 0.0025f;
    float frameThreshold = 0.1f;

    if (IsKeyPressed(KEY_K)) {
        if (self->ctx->tracks.len > MAX_MUSIC_SHOWING) {
            self->indexMove++;
            if (self->indexMove > self->ctx->tracks.len - MAX_MUSIC_SHOWING) {
                self->indexMove = 0;
            }
        }

        self->textOffsetX = 0;
        self->ctx->selected_track = (self->ctx->selected_track + 1) % self->ctx->tracks.len;
        keyHeld = true;
        time = 0;
        StopSelectedTrack(self->ctx);
    } else if (IsKeyPressed(KEY_D)) {
        if (self->ctx->tracks.len > MAX_MUSIC_SHOWING) {
            self->indexMove--;
            if (self->indexMove > self->ctx->tracks.len - MAX_MUSIC_SHOWING) {
                self->indexMove = 0;
            }
        }

        self->textOffsetX = 0;
        self->ctx->selected_track = (self->ctx->selected_track - 1 + self->ctx->tracks.len) % self->ctx->tracks.len;
        keyHeld = true;
        time = 0;
        StopSelectedTrack(self->ctx);
    }

    if (IsKeyDown(KEY_K) || IsKeyDown(KEY_D)) {
        self->textOffsetX = 0;

        time += GetFrameTime();
        if (time >= delay) {
            if (!keyHeld || time >= frameThreshold) {
                if (IsKeyDown(KEY_K)) {
                    if (self->ctx->tracks.len > MAX_MUSIC_SHOWING) {
                        self->indexMove++;
                        if (self->indexMove > self->ctx->tracks.len - MAX_MUSIC_SHOWING) {
                            self->indexMove = 0;
                        }
                    }
                    self->ctx->selected_track = (self->ctx->selected_track + 1) % self->ctx->tracks.len;
                } else if (IsKeyDown(KEY_D)) {

                    if (self->ctx->tracks.len > MAX_MUSIC_SHOWING) {
                        self->indexMove--;
                        if (self->indexMove > self->ctx->tracks.len - MAX_MUSIC_SHOWING) {
                            self->indexMove = 0;
                        }
                    }

                    self->ctx->selected_track = (self->ctx->selected_track - 1 + self->ctx->tracks.len) % self->ctx->tracks.len;
                }
                time = 0;
                keyHeld = true;
            }
        }

        if (delay > maxDelay) {
            delay -= delayReduction;
        }
    } else {
        if (delay < minDelay) {
            delay += delayRecovery;
        }
        keyHeld = false;
    }
}

/**
 * Mengatur visibilitas menu
 */
bool SelectionMenu_IsShow(SelectionMenu *self) {
    if (self->ctx->app_state == APP_SELECT) {
        self->isShow = true;
        return true;
    }

    self->isShow = false;
    return false;
}