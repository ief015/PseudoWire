#pragma once
#include "Platform.h"

namespace sys
{

//Standard (STD) Stopwatch. Uses time.h clock() for time.
class Stopwatch
{
public:
	Stopwatch();
	~Stopwatch();

	// Start/Resume the timer.
	void Start();

	// Halt the timer.
	// Use Start() to resume.
	void Pause();

	// Reset timer down to zero.
	void Reset();

	// Stops the timer and resets.
	void Stop();
	
	// Add to the current elapsed time.
	void AddTime(s64 MS);
	
	// Subtract the current elapsed time.
	void RemoveTime(s64 MS);

	// Time elapsed in total milliseconds.
	s64 ElapsedMS();

	// Time elapsed in fractal seconds.
	f64 ElapsedSec();

	// Is the timer currently running?
	inline bool Running() { return _running; }

private:
	s64 _startTime;
	s64 _pauseTime;
	bool _running;
	bool _initiated;
};

}
