#pragma once
#define WIN32_LEAN_AND_MEAN

#include<windows.h>

namespace usTimer
{
	class HighResolutionTimer
	{
	private:
		LARGE_INTEGER startTime;
		LARGE_INTEGER freq;
		LARGE_INTEGER timer;

	protected:


		double getTime(LARGE_INTEGER time);
	public:
		HighResolutionTimer();

		void startTimer();
		double getTime() {
			return getTime(timer);
		};
		double getTimeFromStart() {
			return getTime(startTime);
		};
		LARGE_INTEGER getResolution() {
			return freq;
		}
	};

	class FrameRate : private HighResolutionTimer
	{
	private:
		float FPS = 0.f;
		float effectiveFPS = 0.f;
		float maxFPS = 0.f;
		double minFrameTime = 0.;
		double maxFrameTime = 0.;
		bool limitFPS = false;

	public:
		FrameRate() {};
		FrameRate(bool limitFPS, float maxFPS, float minFPS) {
			limitFrameRate(limitFPS, maxFPS, minFPS);
		};

		bool calcFrameRate();
		float getFPS() { return FPS; };
		float getEffectiveFPS() { return effectiveFPS; };
		float pixelsPerFrame(float pixelsPerSecond) { return pixelsPerSecond / FPS; };
		void limitFrameRate(bool limitFPS, float maxFPS, float minFPS);
	};
	//methods
}

