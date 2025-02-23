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
void timer_start(Timer*, double second);
bool is_timer_end(Timer*);
// Mengembalikan dalam bentuk detik. Minus berarti timer belum selesai
double time_elapsed(Timer*);

// milidetik ke detik
double ms_to_s(double ms);

// detik ke milidetik
double s_to_ms(double s);

#endif // !TIMER_H
