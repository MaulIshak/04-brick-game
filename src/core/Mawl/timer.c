#include "timer.h"
#include "raylib.h"
// Jangan lupa implement methodnya smh.
// (Andi 16:32)


void timer_start(Timer* t, double second){
    t->timer_start = GetTime();
    t->second = second;
    t->is_started = true;
}

bool is_timer_end(Timer* t){
    return (GetTime() - t->timer_start) >=  + t->second;
}

double time_elapsed(Timer* t) {
    return GetTime() - (t->timer_start + t->second);
}

double ms_to_s(double ms){
    return ms / 1000;
}

double s_to_ms(double s){
    return s * 1000;
}

