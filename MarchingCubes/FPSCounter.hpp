#ifndef __FPSCCOUNTER_HPP__
#define __FPSCCOUNTER_HPP__

#include "Timer.hpp"

class FPSCounter
{
private:
	static Timer fpsTimer;
	static Timer frameTimer;
	static int frameCounter;
	static int fps;

	static double timeSinceLastUpdate;

	static FPSCounter* instance;
	FPSCounter()
	{
		frameCounter = 0;
		fps = 0;
		timeSinceLastUpdate = 0;
	}

public:
	static FPSCounter* getInstance()
	{
		return instance;
	}

	virtual ~FPSCounter()
	{}

	static void init()
	{
		fpsTimer.startTimer();

		frameTimer.startTimer();
		timeSinceLastUpdate = 0;
	}

	static void update()
	{
		timeSinceLastUpdate = frameTimer.elapsedTime();
		frameTimer.resetTimer();

		if (fpsTimer.elapsedTime() > 0.25)
		{
			fpsTimer.resetTimer();
			fps = frameCounter * 4;
			frameCounter = 0;
		}
		else
		{
			frameCounter++;
		}
	}

	static double getTimeSinceLastUpdate()
	{
		return timeSinceLastUpdate;
	}

	static int getFPS()
	{
		return fps;
	}
};
Timer FPSCounter::frameTimer;
Timer FPSCounter::fpsTimer;
int FPSCounter::frameCounter = 0;
int FPSCounter::fps = 0;
double FPSCounter::timeSinceLastUpdate = 0;
FPSCounter* FPSCounter::instance = NULL;
FPSCounter fpsCounter = *FPSCounter::getInstance();

#endif