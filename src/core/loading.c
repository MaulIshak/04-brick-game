#include "loading.h"
#include "scene.h"

void Loading_Draw(Loading *self){
  DrawRectangle(0,0, 10,10, GREEN);
  DrawRectangle(40,0, 10,10, BLUE);
}

void Loading_Update(Loading *self){
  int i = 0;
}

bool Loading_IsShow(Loading *self){
  return true;
}