#include "HiresTimer.h"

#include <iostream>
#include "Platform.h"

using namespace sys;

// http://msdn.microsoft.com/en-us/library/windows/desktop/ms644904(v=vs.85).aspx
// Remarks:
//		On a multiprocessor computer, it should not matter which processor is called.
//		However, you can get different results on different processors due to bugs in the
//		basic input/output system (BIOS) or the hardware abstraction layer (HAL). To specify
//		processor affinity for a thread, use the SetThreadAffinityMask function.

HRTimer::HRTimer()
{
#ifdef IEF_PLATFORM_WIN32
	//Get the tick frequency
	QueryPerformanceFrequency(&frequency);
	startCount.QuadPart = 0;
    endCount.QuadPart = 0;
#else
    startCount.tv_sec = startCount.tv_usec = 0;
    endCount.tv_sec = endCount.tv_usec = 0;
#endif

    stopped = true;
    startTimeInMicroSec = 0.0;
    endTimeInMicroSec = 0.0;
	extraTime = 0.0;
}

void HRTimer::Start()
{
	if(!stopped) return;
	stopped = false;

	//Grab the counter as the start time.
#ifdef IEF_PLATFORM_WIN32
	QueryPerformanceCounter(&startCount);
#else
	gettimeofday(&startCount, NULL);
#endif
}

void HRTimer::Pause()
{
	extraTime += ElapsedMicSD();
	stopped = true;
	
#ifdef WIN32
	startCount.QuadPart = 0;
    endCount.QuadPart = 0;
#else
	startCount.tv_sec = startCount.tv_usec = 0;
    endCount.tv_sec = endCount.tv_usec = 0;
#endif
}

void HRTimer::Stop()
{
	stopped = true;
	
	//Grab the counter.
#ifdef IEF_PLATFORM_WIN32
	QueryPerformanceCounter(&endCount);
#else
	gettimeofday(&endCount, NULL);
#endif
}

void HRTimer::Reset()
{
	//Grab the counter.
#ifdef IEF_PLATFORM_WIN32
	QueryPerformanceCounter(&startCount);
#else
	gettimeofday(&startCount, NULL);
#endif
}

u64 HRTimer::ElapsedMicS()
{
	//Find the time elapsed in microseconds.
#ifdef IEF_PLATFORM_WIN32
    if(!stopped)
        QueryPerformanceCounter(&endCount);

    startTimeInMicroSec = startCount.QuadPart * (1000000.0 / frequency.QuadPart);
    endTimeInMicroSec = endCount.QuadPart * (1000000.0 / frequency.QuadPart);
#else
    if(!stopped)
        gettimeofday(&endCount, NULL);

    startTimeInMicroSec = (startCount.tv_sec * 1000000.0) + startCount.tv_usec;
    endTimeInMicroSec = (endCount.tv_sec * 1000000.0) + endCount.tv_usec;
#endif

	return static_cast<u64>(endTimeInMicroSec - (startTimeInMicroSec-extraTime));
}

f64 HRTimer::ElapsedMicSD()
{
	//Find the time elapsed in microseconds.
#ifdef IEF_PLATFORM_WIN32
    if(!stopped)
        QueryPerformanceCounter(&endCount);

    startTimeInMicroSec = startCount.QuadPart * (1000000.0 / frequency.QuadPart);
    endTimeInMicroSec = endCount.QuadPart * (1000000.0 / frequency.QuadPart);
#else
    if(!stopped)
        gettimeofday(&endCount, NULL);

    startTimeInMicroSec = (startCount.tv_sec * 1000000.0) + startCount.tv_usec;
    endTimeInMicroSec = (endCount.tv_sec * 1000000.0) + endCount.tv_usec;
#endif

	return endTimeInMicroSec - (startTimeInMicroSec-extraTime);
}
