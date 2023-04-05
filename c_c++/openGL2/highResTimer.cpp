#include "highResTimer.h"

void getTimerFrequency(HIGH_RES_TIMER_FREQ* timerFreq, TIMER* keyTime) 
{ 
	QueryPerformanceFrequency(timerFreq);
	QueryPerformanceCounter(keyTime);
}

double getTimeNs(HIGH_RES_TIMER_FREQ timerFreq, TIMER* keyTime)
{
	TIMER time;
	QueryPerformanceCounter(&time);
	double nanoseconds = (time.QuadPart - keyTime->QuadPart)*1000000000 / timerFreq.QuadPart;
	keyTime->QuadPart = time.QuadPart;
	return nanoseconds;
}