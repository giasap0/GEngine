#include "commonHeaders.h"
#include <GEngineCore.h>
#include "GHighResTimer.h"

GMath::uint64 GHighResTimer::_frequency = 250000;	//must be init. I chose arbitrary 250MHz (a slow machine)


GHighResTimer::GHighResTimer(double startSeconds) : _timeElapsed(startSeconds),_scale(1.0f),_isPaused(false)
{
	startCounting();	//get the stamp of number of cycles
}
GHighResTimer::GHighResTimer(GMath::GTime startTime) : _timeElapsed(startTime),_scale(1.0f),_isPaused(false)
{
	startCounting();
}

void GHighResTimer::init()
{
	LARGE_INTEGER f;
	QueryPerformanceFrequency(&f);
	_frequency = f.QuadPart;
	if (_frequency==0)
	{
		DBG_OUTPT("QueryPerformanceFrequency   ---- frequency == 0");
	}
}

void GHighResTimer::startCounting()
{
	LARGE_INTEGER cyc;
	QueryPerformanceCounter(&cyc);
	_cycles = cyc.QuadPart;
}

GMath::GTime GHighResTimer::update()
{
	if(_isPaused)
		return GMath::GTime();
	LARGE_INTEGER newStamp;
	QueryPerformanceCounter(&newStamp);
	GMath::uint64 deltaCycles = newStamp.QuadPart - _cycles;
	GMath::GTime deltaTime = cyclesToTime(deltaCycles);
	if(deltaTime.inSeconds()>5.0f)
		deltaTime.setTimeinSeconds(1.0/30.0);
	deltaTime*=_scale;
	_timeElapsed += ( deltaTime);
	return deltaTime;
}
GMath::GTime GHighResTimer::unsafeUpdate()
{
	if(_isPaused)
		return GMath::GTime();
	LARGE_INTEGER newStamp;
	QueryPerformanceCounter(&newStamp);
	GMath::uint64 deltaCycles = newStamp.QuadPart - _cycles;
	GMath::GTime deltaTime = cyclesToTime(deltaCycles);
	deltaTime*=_scale;
	_timeElapsed += ( deltaTime);
	return deltaTime;
}
void GHighResTimer::singleStep()
{
	if(!_isPaused)
		return;
	double idealDelta = 1.0/30.0;
	_timeElapsed +=GMath::GTime(idealDelta*_scale);
}
