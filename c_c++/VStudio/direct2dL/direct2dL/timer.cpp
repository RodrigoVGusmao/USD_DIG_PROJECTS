#include "timer.h"

#include<timeapi.h>

#include<exception>
#include<limits>

#include"error.h"

//HighResolutionTimer
usTimer::HighResolutionTimer::HighResolutionTimer()
{
	std::exception except(nullptr);
	//if(freq.QuadPart != 0)
	if (QueryPerformanceFrequency(&freq) == FALSE)
		goto _GOTO_US_ERR_HIGH_RES_TIMER_CREATE;

	if (QueryPerformanceCounter(&startTime) == FALSE)
		goto _GOTO_US_ERR_HIGH_RES_TIMER_GET;

	timer = startTime;

	return;
_GOTO_US_ERR_HIGH_RES_TIMER_CREATE:
	if (except.what() == nullptr)
		except = std::exception(US_ERR_HIGH_RES_TIMER_CREATE_MSG);

_GOTO_US_ERR_HIGH_RES_TIMER_GET:
	if (except.what() == nullptr)
		except = std::exception(US_ERR_HIGH_RES_TIMER_GET_MSG);

	throw except;
}

void usTimer::HighResolutionTimer::startTimer()
{
	if (QueryPerformanceCounter(&timer) == FALSE)
		throw std::exception(US_ERR_HIGH_RES_TIMER_GET_MSG);
}

double usTimer::HighResolutionTimer::getTime(LARGE_INTEGER time)
{
	LARGE_INTEGER currentTime;
	if (QueryPerformanceCounter(&currentTime) == FALSE)
		throw std::exception(US_ERR_HIGH_RES_TIMER_GET_MSG);

	double timeElapsed;
	timeElapsed = static_cast<double>((currentTime.QuadPart - time.QuadPart)) / static_cast<double>(freq.QuadPart);
	return timeElapsed;
}

//FrameRate
bool usTimer::FrameRate::calcFrameRate()
{
	double time = getTime();

	if (limitFPS == true && minFrameTime > time)
	{
		DWORD sleepTime = static_cast<DWORD>((minFrameTime - time) * 1000);
		timeBeginPeriod(1);
		Sleep(sleepTime);
		timeEndPeriod(1);
		return false;
	}

	effectiveFPS = static_cast<float>(1 / time);

	if (time > maxFrameTime)
		time = maxFrameTime;

	FPS = static_cast<float>(1 / time);

	startTimer();
	return true;
}

#undef max
void usTimer::FrameRate::limitFrameRate(bool limitFPS, float maxFPS, float minFPS)
{
	this->limitFPS = limitFPS;
	this->maxFPS = maxFPS;
	minFrameTime = 1.0 / maxFPS;
	maxFrameTime = minFPS == 0 ? std::numeric_limits<double>::max() : 1.0 / minFPS;
}
#define max(a,b)	(((a) > (b)) ? (a) : (b))