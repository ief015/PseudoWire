#pragma once
#include "Platform.h"

#ifdef IEF_PLATFORM_WIN32
	#include <windows.h>
#else	//MAC_OS + LINUX/UNIX
	#include <sys/time.h>
#endif

namespace sys
{

//OS-based High Resolution Timer.
//Similar to the Stopwatch, except this relies on the Operating System timers (sys::Stopwatch uses STD timers).
//The resolution for this timer is usually as low as 1 Microsecond (for most OSs) for extreme precision.
class HRTimer
{
private:
	f64 startTimeInMicroSec;
	f64 endTimeInMicroSec;
	f64 extraTime;
	bool stopped;
	
#ifdef IEF_PLATFORM_WIN32
	LARGE_INTEGER frequency; // ticks per second
	LARGE_INTEGER startCount;
	LARGE_INTEGER endCount;
#else
	timeval startCount;
	timeval endCount;
#endif
	
public:
	HRTimer();

	//Start the timer.
	void Start();
	
	//Halt the timer. Use Start() to resume.
	void Pause();

	//Stop and reset the timer. Ready to use Start() to restart.
	void Stop();

	//Reset the timer to zero without stopping.
	void Reset();

	//Add extra time.
	inline void AddMicroSeconds(f64 Microseconds) { extraTime += Microseconds; }
	//Remove extra time.
	inline void RemoveMicroSeconds(f64 Microseconds) { extraTime -= Microseconds; }
	//Add extra time.
	inline void AddMilliSeconds(f64 Milliseconds) { extraTime += Milliseconds*1000.0; }
	//Remove extra time.
	inline void RemoveMilliSeconds(f64 Milliseconds) { extraTime -= Milliseconds*1000.0; }
	//Add extra time.
	inline void AddSeconds(f64 Seconds) { extraTime += Seconds*1000000.0; }
	//Remove extra time.
	inline void RemoveSeconds(f64 Seconds) { extraTime -= Seconds*1000000.0; }

	//Get the elapsed microseconds. (1,000,000 = 1 second)
	u64 ElapsedMicS();

	//Get the elapsed microseconds. [As Double] (1,000,000 = 1 second)
	f64 ElapsedMicSD();

	//Get the elapsed milliseconds. (1,000 = 1 second)
	inline f64 ElapsedMS() { return (this->ElapsedMicS() * 0.001); }

	//Get the elapsed seconds.
	inline f64 ElapsedSec() { return (this->ElapsedMicS() * 0.000001); }

	inline bool IsRunning() const { return stopped; }
};

}