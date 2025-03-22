#include "flying_object.h"
#include "context.h"
#include "math.h"
#include "raymath.h"
#include <stdio.h>

FlyingObjects FlyingObject_Create(AppContext *ctx) {
    FlyingObjects objs = {0};
    objs.AjawTexture = LoadTexture("resources/texture/ajaw.png");
    objs.CitlalinTexture = LoadTexture("resources/texture/Citlalin.png");
    for(int i = 0; i < FLYING_OBJECT_COUNT; i++) {
        objs.objs[i] = ObjectCreate(ctx->screen_width, ctx->screen_height);
        objs.objs[i].pos.x += objs.objs[i].target.x / (i + 1);
        objs.objs[i].pos.y -= objs.objs[i].target.y / (i + 1);
        objs.objs[i].rotation = (float)GetRandomValue(0, 360);
    }
    objs.ctx = ctx;
    return objs;
}

Vector2 FlyingObjectRandomTarget(int width, int height) {
    Vector2 target;
    if(GetRandomValue(0,1) == 1) {
        target.x = width + 40;
        target.y = GetRandomValue(0, height / 2); 
    }else {
        target.y = - 40;
        target.x = GetRandomValue(width / 2, width) ;
    }

    return target;
}

Vector2 FlyingObjectRandomPos(int height){
    Vector2 pos;
    if(GetRandomValue(0, 1) == 0 ) {
        pos.x = GetRandomValue(0, 300);
        pos.y = height;
    }else {
        pos.x = 0;
        pos.y = height - GetRandomValue(0, 400);
    }
    return pos;
}

FlyingObject ObjectCreate(int width, int height) {
    FlyingObject obj;
    obj.target = FlyingObjectRandomTarget(width, height);
    obj.pos = FlyingObjectRandomPos(height);

    int rand = GetRandomValue(0, 20);

    if(rand >= 0 && rand < 9) {
        obj.type = OBJECT_TRIANGLE;
    }else if (rand >= 9 && rand < 18) {
        obj.type = OBJECT_CIRCLE;
    }
    else if (rand >= 18) {
        if(GetRandomValue(0,1) == 0) {
            obj.type = OBJECT_AJAW;
        }else {
            obj.type = OBJECT_CITLALIN;
        }
    }
    obj.alpha = 0;
    obj.alive = true;
    return obj;
}


void FlyingObject_Update(FlyingObjects *self, AppContext *ctx) {
    for(int i = 0; i < FLYING_OBJECT_COUNT; i++) {
        FlyingObject *obj = &self->objs[i];
        Vector2 dir;
        double length;
        if (!obj->alive){
            *obj = ObjectCreate(ctx->screen_width, ctx->screen_height);
        }
        
        if(obj->alpha < 1) {
            obj->alpha += 0.05;
        }

        obj->rotation += 0.005;
        
        dir.x = obj->target.x - obj->pos.x;
        dir.y = obj->target.y - obj->pos.y;
        
        length = sqrt(pow(dir.x, 2) + pow(dir.y, 2)); 
        if(obj->pos.x > ctx->screen_width + 20 || obj->pos.y < - 20  ) {
            obj->alive = false;
        }

        dir.x /= length;
        dir.y /= length;

        obj->pos.x = obj->pos.x + dir.x * FLYING_OBJECT_SPEED;
        obj->pos.y = obj->pos.y + dir.y * FLYING_OBJECT_SPEED;
    }
}

void FlyingObject_Draw(FlyingObjects *self) {
    for(int i = 0; i < FLYING_OBJECT_COUNT; i++) {
        FlyingObject *obj = &self->objs[i];
        switch (obj->type)
        {
        case OBJECT_TRIANGLE: {
            Vector2 v1,v2,v3;
            float size = 50;

            float height = size *  (sqrtf(3)/2);

            v1.x = obj->pos.x;
            v1.y = obj->pos.y - height / 2;

            v2.x = obj->pos.x - size / 2;
            v2.y = obj->pos.y + height / 2;

            v3.x = obj->pos.x + size / 2;
            v3.y = obj->pos.y + height / 2;

            v1 = Vector2Add(Vector2Rotate(Vector2Subtract(v1, obj->pos), obj->rotation), obj->pos);
            v2 = Vector2Add(Vector2Rotate(Vector2Subtract(v2, obj->pos), obj->rotation), obj->pos);
            v3 = Vector2Add(Vector2Rotate(Vector2Subtract(v3, obj->pos), obj->rotation), obj->pos);

            

            DrawTriangle(v1, v2, v3, Fade(BLUE, 0.5));
            
            break;
        }
        case OBJECT_CIRCLE:
            DrawCircleLinesV(obj->pos, 30, BLUE);
            break;
        case OBJECT_AJAW:
            DrawTextureEx(self->AjawTexture, obj->pos, obj->rotation * 2, 0.025, WHITE);
            break;
        case OBJECT_CITLALIN:
            DrawTextureEx(self->CitlalinTexture, obj->pos, obj->rotation * 2, 0.025, WHITE);
            break;
        default:
            break;
        }
        
    }

    
}