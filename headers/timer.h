#ifndef TIMER_H
#define TIMER_H

//High resolution timer using QueryPerformanceFrequency

#include <windows.h>
#include "timer_struct.h"

// Timestep Example Source Code
// Copyright (c) Glenn Fiedler 2004
// http://www.gaffer.org/articles

void StartCounter();
double GetCounter();
void onQuit();
State interpolate(const State &previous, const State &current, float alpha);
float acceleration(const State &state, float t);
Derivative evaluate(const State &initial, float t);
Derivative evaluate(const State &initial, float t, float dt, const Derivative &d);
void integrate(State &state, float t, float dt);
void runTimeStep();

#endif