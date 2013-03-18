#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include <Windows.h>

class Timer
{
public:
	LARGE_INTEGER	frequency;
	LARGE_INTEGER	start;
	LARGE_INTEGER	end;

	Timer()
	{
		QueryPerformanceFrequency(&frequency);
	}

	void startTimer()
	{
		QueryPerformanceCounter(&start);
	}

	void stopTimer()
	{
		QueryPerformanceCounter(&end);
	}

	void resetTimer()
	{
		stopTimer();
		start = end;
	}

	double totalTime() // returns end-start
	{
		return ((double)(end.QuadPart-start.QuadPart)/(double)(frequency.QuadPart));
	}

	double elapsedTime() // returns now-start
	{
		LARGE_INTEGER t;
		QueryPerformanceCounter(&t);

		return ((double)(t.QuadPart-start.QuadPart)/(double)(frequency.QuadPart));
	}
};

#endif