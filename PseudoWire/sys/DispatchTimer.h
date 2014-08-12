#pragma once

#include "Stopwatch.h"
#include "Platform.h"
namespace sys
{

//Does not use the HRTimer. Uses Stopwatch.
class DispatchTimer
{
private:
	Stopwatch tdStopWatch;
	
protected:
	long long tdIntervalMS;
	
public:
	DispatchTimer()
	: tdIntervalMS(1000) //default to 1 second
	{
		tdStopWatch = Stopwatch();
	}

	DispatchTimer(s64 interval, bool autoStart = true)
	{
		tdIntervalMS = interval;
		tdStopWatch = Stopwatch();

		if(autoStart)
			tdStopWatch.Start();
	}

	// Start the timer.
	inline void Start()
	{
		tdStopWatch.Start();
	}
	
	// Halt the timer.
	inline void Pause()
	{
		tdStopWatch.Pause();
	}
	
	// Stop and reset the timer back to zero.
	inline void Stop()
	{
		tdStopWatch.Stop();
	}

	// Get current elapsed milliseconds.
	inline long long Elapsed()
	{
		return tdStopWatch.ElapsedMS();
	}

	// Check if ready to dispatch. Returns true if ready to dispatch.
	//
	// Example:
	//
	// while(dispatchTimer.Dispatch())
	// {
	// 	doActionOnDispatch();
	// }
	bool Dispatch()
	{
		if(tdStopWatch.ElapsedMS() >= tdIntervalMS)
		{
			tdStopWatch.RemoveTime(tdIntervalMS);
			return true;
		}
		
		return false;
	}
};


}