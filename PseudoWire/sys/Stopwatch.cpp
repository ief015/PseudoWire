#include "Stopwatch.h"
#include "Platform.h"

#include <time.h>

using namespace sys;

Stopwatch::Stopwatch()
: _running(false)
, _initiated(false)
, _startTime(0)
, _pauseTime(0)
{

}

Stopwatch::~Stopwatch()
{

}

void Stopwatch::Start()
{
	if(_running) return;

	if(_initiated)
	{
		_startTime -= _pauseTime;
		_startTime += clock();
	}
	else
	{
		_startTime = clock();
		_initiated = true;
	}

	_running = true;
}

void Stopwatch::Stop()
{
	_running = false;
	_initiated = false;
	_startTime = 0;
	_pauseTime = 0;
}

void Stopwatch::Pause()
{
	_pauseTime = clock();
	_running = false;
}

void Stopwatch::Reset()
{
	_startTime = clock();
}

void Stopwatch::AddTime(s64 MS)
{
	_startTime -= MS;
}

void Stopwatch::RemoveTime(s64 MS)
{
	_startTime += MS;
}

s64 Stopwatch::ElapsedMS()
{
	if(!_initiated) return 0;
	if(_running)
		return clock() - _startTime;
	else
		return _pauseTime - _startTime;
}

f64 Stopwatch::ElapsedSec()
{
	if(!_initiated) return 0.0;
	if(_running)
		return static_cast<f64>(clock() - _startTime) / CLOCKS_PER_SEC;
	else
		return static_cast<f64>(_pauseTime - _startTime) / CLOCKS_PER_SEC;
}