#include "raylib.h"
#include "context.h"
#include "macro.h"
#include "gameplay.h"
#include <string.h>
#include <stdio.h>
#include "timer.h"
#include "progress_bar.h"
#include "score.h"
#include "linked-list.h"


// Variable global untuk meyimpan texture background yang dirandom


void gp_draw(Gameplay* self){
  // Variable lokal
  char* control[LINE_COUNT]= {"D", "F", "J", "K"};

    Rectangle rec = {
      0,0,self->width, self->ctx->screen_height
    };
    // Jalur note
    Rectangle rec2 = {
      self->padPositions[0].x, 0,self->padPositions[3].x + 10, self->ctx->screen_height
    };
    Rectangle rec3 = {
      self->padPositions[0].x, self->padPositions[0].y, self->padSize , self->padSize
    };

    // Draw background
    DrawTexture(self->bg,0,0,WHITE);

    // Flash effect background
    DrawRectangleRec(rec, Fade(WHITE, self->alpha/255 - 0.7f));

    // Background hitam transparan jalur note
    DrawRectangleRec(rec2, Fade(BLACK, .5f));

    // _drawAccZone(self);

    DrawLine(self->width, 0, self->width, self->ctx->screen_height, BLACK);
    NodeAddress currentNode = self->textureToDrawHead;
    // // Draw pad
    // // Use linked list instead of array for pad textures
    int i = 0;
    while(currentNode != NULL){
      Texture2D texture = *((Texture2D*)(currentNode->info));
      Vector2 pos = self->padPositions[i];
      // Draw pad
      DrawTextureEx(texture, pos, 0, .16f, (Color){ 240, 240, 240, 255 });
      // Draw guide text
      DrawTextEx(self->ctx->font, control[i], (Vector2){pos.x + self->padSize/2 - 7, pos.y - 30}, 40, 1,WHITE);
      currentNode = currentNode->next;
      i++;
    }
  
    // Progress Bar
    DrawProgressBar(&self->progressBar);

    // Life Bar
    _drawLifeBar(self);
    
  }
  void gp_update(Gameplay* self){
    _updateLifeBar(self);
    _padFeedbackHandler(self);

    if(!self->isBackgroundLoaded){
      self->bg = _getRandomBg(self);
      self->isBackgroundLoaded = true;
    }
    // Mulai waktu game
    if(!self->timer.is_started){
      self->startGameTime = GetTime();
      timer_start(&(self->timer), 3);
    }
    if(is_timer_end(&(self->timer))){
      _UpdateGameTime(self);
    }
    // if(!self->isPlaying) return;
    UpdateProgressBar(&self->progressBar, self);

    if(self->life >= 100){
      self->life = 100;
    }

  }

bool gp_isShow(Gameplay* self){
  if(self->ctx->app_state == APP_PLAYING){
    return true;
  }
  return false;
}

void InitGameplay(Gameplay *gameplay, AppContext *ctx){

  _AllListInit(gameplay);

  // node_append(gameplay->padActiveTexturePathHead, "resources/texture/Pad_Active-01.png");
  // printf("Texture init: %s\n", ((char*)gameplay->padActiveTexturePathHead->info));
  gameplay->ctx = ctx;
  gameplay->width = gameplay->ctx->screen_width;
  for (int i = 0; i < LINE_COUNT; i++)
  {
    gameplay->padPositions[i].x = gameplay->ctx->screen_width/6 * i+ gameplay->ctx->screen_width/8;
    gameplay->padPositions[i].y = 48;
  }
  gameplay->gameTime = 0;
  _LoadNoteTexture(gameplay);
  gameplay->timer = (Timer){false, 0,0};
  gameplay->gameTimeOffset = 2000;

  InitProgressBar(&gameplay->progressBar, 0, 0, gameplay->width, 10, SECONDARY_COLOR);
  gameplay->padSize = 512*0.16f +5;
  gameplay->alpha = 0;
  gameplay->background[0] = LoadTexture("resources/background/Background-01.png");
  gameplay->background[1] = LoadTexture("resources/background/Background-02.png");
  gameplay->background[2] = LoadTexture("resources/background/Background-03.png");
  gameplay->maxLife = 100;
  gameplay->life = gameplay->maxLife;
  gameplay->lifeBar.height = 20;
  gameplay->lifeBar.width = 80;

  gameplay->lifeBar.outlineRec = (Rectangle){.height=gameplay->lifeBar.height, .width= gameplay->lifeBar.width, .x=GetScreenWidth()-GetScreenWidth()/6, .y=GetScreenHeight()/4};
  gameplay->lifeBar.lifeRec = (Rectangle){.height=gameplay->lifeBar.height, .width= gameplay->lifeBar.width, .x=GetScreenWidth()-GetScreenWidth()/6, .y=GetScreenHeight()/4};
  // meledak = LoadTexture("resources/texture/Meledak-2.png");
  // frameRec = (Rectangle){ 0.0f, 0.0f, (float)meledak.width/8, (float)meledak.height };
  gameplay->isBackgroundLoaded = false;
  // gameplay->isPlaying = false;
}
void _AllListInit(Gameplay* gameplay){
  gameplay->padActiveTexturePathHead = NULL;
  node_create(&gameplay->padActiveTexturePathHead);
  node_init(&gameplay->padActiveTexturePathHead, "resources/texture/Pad_Active-01.png");
  node_append(&gameplay->padActiveTexturePathHead, "resources/texture/Pad_Active-02.png");
  node_append(&gameplay->padActiveTexturePathHead, "resources/texture/Pad_Active-03.png");
  node_append(&gameplay->padActiveTexturePathHead, "resources/texture/Pad_Active-04.png");

  gameplay->padPassiveTexturePathHead = NULL;
  node_create(&gameplay->padPassiveTexturePathHead);
  node_init(&gameplay->padPassiveTexturePathHead, "resources/texture/Pad-01.png");
  node_append(&gameplay->padPassiveTexturePathHead, "resources/texture/Pad-02.png");
  node_append(&gameplay->padPassiveTexturePathHead, "resources/texture/Pad-04.png");
  node_append(&gameplay->padPassiveTexturePathHead, "resources/texture/Pad-03.png");

  gameplay->textureToDrawHead = NULL;
  gameplay->padActiveTextureToLoadHead = NULL;
  gameplay->padPassiveTextureToLoadHead = NULL;
}

void _LoadNoteTexture(Gameplay*self){
  // Load pad passive texture
  NodeAddress cur = self->padPassiveTexturePathHead;
  while(cur != NULL){
    Texture2D *texture = malloc(sizeof(Texture2D));
    *texture = LoadTexture((char*)cur->info);
    node_append(&self->padPassiveTextureToLoadHead, (void*)texture);
    cur = cur->next;
  }
  // Load pad active texture
  cur = self->padActiveTexturePathHead;
  while(cur != NULL){
    Texture2D *texture = malloc(sizeof(Texture2D));
    *texture = LoadTexture((char*)cur->info);
    node_append(&self->padActiveTextureToLoadHead, (void*)texture);
    cur = cur->next;
  }
  // Initialize texture to draw
  cur = self->padPassiveTextureToLoadHead;
  while(cur != NULL){
    Texture2D *texture = (Texture2D*)cur->info;
    node_append(&self->textureToDrawHead, (void*)texture);
    cur = cur->next;
  }
  
}

void _UpdateGameTime(Gameplay* self){
  float currentTime = GetTime();
  self->gameTime =s_to_ms(currentTime - self->startGameTime - 3);
  // printf("%.2f\n", self->gameTime);
}

void _drawAccZone(Gameplay* self){
  Rectangle recPerfect = {
    self->padPositions[0].x, self->padPositions[0].y + 20, self->padPositions[3].x + 10 , self->padSize - 20*2
  };
  Rectangle recGood = {
    self->padPositions[0].x, self->padPositions[0].y, self->padPositions[3].x + 10 , self->padSize
  };
  Rectangle recMiss = {
    self->padPositions[0].x, self->padPositions[0].y - 20, self->padPositions[3].x + 10 , self->padSize + 20 + 20
  };

  DrawRectangleRec(recPerfect, Fade(GREEN, 1));
  DrawRectangleRec(recGood, Fade(YELLOW, 0.2f));
  DrawRectangleRec(recMiss, Fade(RED, 0.2f));
}

Texture2D _getRandomBg(Gameplay* self){
  int rand = GetRandomValue(0, 2);
  return self->background[rand];
}


void _drawLifeBar(Gameplay* self){
  DrawText("Life",self->lifeBar.outlineRec.x, self->lifeBar.outlineRec.y - 30, 30, BLACK);
  DrawRectangleRec(self->lifeBar.outlineRec, BLACK);
  DrawRectangleRec(self->lifeBar.lifeRec, GREEN);
}

void _updateLifeBar(Gameplay* self){
  self->lifeBar.lifeRec.width = (self->life/self->maxLife) * self->lifeBar.width;
}

void UpdateLife(Gameplay *self, Accuracy acc){
  if(acc == MISS){
    self->life -= 10;
  }else if(acc == PERFECT){
    self->life += 5;
  }
}

void _padFeedbackHandler(Gameplay* self){
  // DOWN ARROW (MIDDLE LEFT)
if (IsKeyDown(KEY_J) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_1)) {
    NodeAddress activeNode = self->padActiveTextureToLoadHead;
    NodeAddress textureNode = self->textureToDrawHead;

    for (int i = 0; i < DOWN_INDEX && activeNode != NULL && textureNode != NULL; i++) {
        activeNode = activeNode->next;
        textureNode = textureNode->next;
    }

    if (activeNode != NULL && textureNode != NULL) {
        textureNode->info = activeNode->info;
    }
} else {
    NodeAddress passiveNode = self->padPassiveTextureToLoadHead;
    NodeAddress textureNode = self->textureToDrawHead;

    for (int i = 0; i < DOWN_INDEX && passiveNode != NULL && textureNode != NULL; i++) {
        passiveNode = passiveNode->next;
        textureNode = textureNode->next;
    }

    if (passiveNode != NULL && textureNode != NULL) {
        textureNode->info = passiveNode->info;
    }
}

// LEFT ARROW (LEFT)
if (IsKeyDown(KEY_D) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_TRIGGER_2)) {
    NodeAddress activeNode = self->padActiveTextureToLoadHead;
    NodeAddress textureNode = self->textureToDrawHead;

    for (int i = 0; i < LEFT_INDEX && activeNode != NULL && textureNode != NULL; i++) {
        activeNode = activeNode->next;
        textureNode = textureNode->next;
    }

    if (activeNode != NULL && textureNode != NULL) {
        textureNode->info = activeNode->info;
    }
} else {
    NodeAddress passiveNode = self->padPassiveTextureToLoadHead;
    NodeAddress textureNode = self->textureToDrawHead;

    for (int i = 0; i < LEFT_INDEX && passiveNode != NULL && textureNode != NULL; i++) {
        passiveNode = passiveNode->next;
        textureNode = textureNode->next;
    }

    if (passiveNode != NULL && textureNode != NULL) {
        textureNode->info = passiveNode->info;
    }
}

// UP ARROW (MIDDLE RIGHT)
if (IsKeyDown(KEY_F) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_TRIGGER_1)) {
    NodeAddress activeNode = self->padActiveTextureToLoadHead;
    NodeAddress textureNode = self->textureToDrawHead;

    for (int i = 0; i < UP_INDEX && activeNode != NULL && textureNode != NULL; i++) {
        activeNode = activeNode->next;
        textureNode = textureNode->next;
    }

    if (activeNode != NULL && textureNode != NULL) {
        textureNode->info = activeNode->info;
    }
} else {
    NodeAddress passiveNode = self->padPassiveTextureToLoadHead;
    NodeAddress textureNode = self->textureToDrawHead;

    for (int i = 0; i < UP_INDEX && passiveNode != NULL && textureNode != NULL; i++) {
        passiveNode = passiveNode->next;
        textureNode = textureNode->next;
    }

    if (passiveNode != NULL && textureNode != NULL) {
        textureNode->info = passiveNode->info;
    }
}

// RIGHT ARROW (RIGHT)
if (IsKeyDown(KEY_K) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2)) {
    NodeAddress activeNode = self->padActiveTextureToLoadHead;
    NodeAddress textureNode = self->textureToDrawHead;

    for (int i = 0; i < RIGHT_INDEX && activeNode != NULL && textureNode != NULL; i++) {
        activeNode = activeNode->next;
        textureNode = textureNode->next;
    }

    if (activeNode != NULL && textureNode != NULL) {
        textureNode->info = activeNode->info;
    }
} else {
    NodeAddress passiveNode = self->padPassiveTextureToLoadHead;
    NodeAddress textureNode = self->textureToDrawHead;

    for (int i = 0; i < RIGHT_INDEX && passiveNode != NULL && textureNode != NULL; i++) {
        passiveNode = passiveNode->next;
        textureNode = textureNode->next;
    }

    if (passiveNode != NULL && textureNode != NULL) {
        textureNode->info = passiveNode->info;
    }
}
}