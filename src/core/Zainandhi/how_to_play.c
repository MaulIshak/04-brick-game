#include <stdio.h>
#include "raylib.h"
#include "how_to_play.h"

HowToPlayManager InitHowToPlay(AppContext *ctx)
{
    HowToPlayManager how = {0};
    how.ctx = ctx;
    // how.currentIndex = 0;
    how.isLoaded = false;

    ImageNode *prev = NULL;
    for (int i = 0; i < NUM_PAGES; i++)
    {
        // how.pages[i] = LoadTexture(imagePaths[i]);
        // if (how.pages[i].id == 0)
        // {
        //     TraceLog(LOG_ERROR, "HOW_TO_PLAY: Failed to load texture: %s", imagePaths[i]);
        //     how.isLoaded = false;
        //     break;
        // }
        ImageNode *node = (ImageNode *)malloc(sizeof(ImageNode));
        if (!node)
        {
            TraceLog(LOG_ERROR, "HOW_TO_PLAY: Failed to allocate memory for node");
            return how;
        }

        node->texture = LoadTexture(imagePaths[i]);
        if (node->texture.id == 0)
        {
            TraceLog(LOG_ERROR, "HOW_TO_PLAY: Failed to load texture: %s", imagePaths[i]);
            free(node);
            UnloadHowToPlayImages(&how);
            return how;
        }

        node->next = NULL;
        node->prev = prev;

        if (prev)
        {
            prev->next = node;
        }
        else
        {
            how.head = node;
        }
        prev = node;
    }

    if (how.head && prev)
    {
        how.head->prev = prev;
        prev->next = how.head;
        how.current = how.head;
        how.isLoaded = true;
    }
    else
    {
        TraceLog(LOG_ERROR, "HOW_TO_PLAY: No pages loaded");
    }

    return how;
}

void UpdateHowToPlay(HowToPlayManager *how)
{
    if (!how->isLoaded)
        return;

    float scale = 0.3f;
    // Texture2D texture = how->pages[how->currentIndex];

    Vector2 position = {
        // how->ctx->screen_width / 2 - (texture.width * scale) / 2,
        // how->ctx->screen_height / 2 - (texture.height * scale) / 2 
        how->ctx->screen_width / 2 - (how->current->texture.width * scale) / 2,
        how->ctx->screen_height / 2 - (how->current->texture.height * scale) / 2
    };

    // DrawTextureEx(texture, position, 0.0f, scale, WHITE);
    DrawTextureEx(how->current->texture, position, 0.0f, scale, WHITE);

    const char *titleText = "TUTORIAL";
    DrawText(titleText, how->ctx->screen_width / 2 - MeasureText(titleText, 40) / 2, position.y - 60, 40, BLACK);

    const char *navText = "<- Left | Right ->";
    DrawText(navText, how->ctx->screen_width / 2 - MeasureText(navText, 20) / 2, how->ctx->screen_height - 80, 20, GRAY);

    DrawText("B to return", how->ctx->screen_width - 150, how->ctx->screen_height - 40, 20, GRAY);
}

void DrawHowToPlay(HowToPlayManager *how)
{
    if (!how->isLoaded)
        return;

    if (IsKeyPressed(KEY_RIGHT))
    {
        // how->currentIndex = (how->currentIndex + 1) % NUM_PAGES;
        how->current = how->current->next;
    }
    if (IsKeyPressed(KEY_LEFT))
    {
        // how->currentIndex = (how->currentIndex - 1 + NUM_PAGES) % NUM_PAGES;
        how->current = how->current->prev;
    }
    if (IsKeyPressed(KEY_B))
    {
        how->ctx->app_state = APP_LOADING;
    }
}

bool IsShowHowToPlay(HowToPlayManager *how)
{
    return (how->ctx->app_state == APP_HOW_TO_PLAY);
}

void UnloadHowToPlayImages(HowToPlayManager *how)
{
    if (!how->isLoaded)
        return;

    // for (int i = 0; i < NUM_PAGES; i++)
    // {
    //     UnloadTexture(how->pages[i]);
    // }

    ImageNode *start = how->head;
    ImageNode *current = how->head;

    if (current != NULL)
    {
        while (true)
        {
            ImageNode *next = current->next;
            UnloadTexture(current->texture);
            free(current);

            if (next == start)
                break;
            current = next;
        }
    }

    how->isLoaded = false;
}