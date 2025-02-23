#pragma once
#include <stdbool.h>
#include "raylib.h"

#ifndef TIMER_H
#define TIMER_H

typedef struct Timer {
  bool is_started;
  double timer_start;
  double second;
} Timer;

void timer_update(Timer*);
void timer_start(Timer*);


#endif // !TIMER_H
