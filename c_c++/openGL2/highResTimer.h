#pragma once

#include<Windows.h>

typedef LARGE_INTEGER HIGH_RES_TIMER_FREQ;
typedef LARGE_INTEGER TIMER;

void getTimerFrequency(HIGH_RES_TIMER_FREQ* timerFreq, TIMER* keyTime);
double getTimeNs(HIGH_RES_TIMER_FREQ timerFreq, TIMER* keyTime);