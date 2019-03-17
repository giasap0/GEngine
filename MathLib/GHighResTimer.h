#pragma once
#ifndef GHIGHRESTIMER_H
#define GHIGHRESTIMER_H

#include "GTime.h"

/*****************************     HOW TO USE     ****************************
**	GHighResTimer tmr;														**
**	std::cout << "not initialized = " << tmr.cyclesPerSecond() <<"\n";		**
**	GHighResTimer::init();													**
**	std::cout << "    initialized = " << tmr.cyclesPerSecond() <<"\n";		**
**	int x = 0;																**	
**	while(x<100000)															**
**	{																		**
**		tmr.startCounting();												**
**		++x;																**
**		//std::cout << tmr.update().inSeconds()<< "\n";	//like10^-5			**
**		tmr.update();														**
**	}																		**
**	std::cout << tmr.timeElapsed().inSeconds() << "\n";	//like 5msec		**
**	std::cout << tmr.timeElapsed() << "\n";									**
******************************************************************************/

class GHighResTimer
{
public:
	GHighResTimer() : _cycles(0), _timeElapsed(0), _scale(1.0f), _isPaused(false)	{}
	explicit GHighResTimer(double startSeconds);
	explicit GHighResTimer(GMath::GTime startTime);
	virtual ~GHighResTimer(void)									{}

	//get system cpu frequency
	static void init();
	inline void pause(bool setPaused)								{_isPaused= setPaused;}
	inline void reset()												{_timeElapsed.setTimeinSeconds(0.0f);}

	//call this when you want to start counting
	void startCounting();
	// This function update timeElapsed since the last call of startCounting(), return deltaTime
	GMath::GTime update();
	//like update but do not control deltaTime
	GMath::GTime unsafeUpdate();
	// Add one ideal frame interval (1/30s = 0.03sec) [scaled] - works only if timer is paused;
	void singleStep();

	inline bool isPaused() const									{return _isPaused;}
	inline GMath::Real timeScale() const							{return _scale;}
	//<1 slow down, >1 fast forward
	inline void setTimeScale(GMath::Real newScale)					{_scale=newScale;}
	//CPU frequency
	static inline GMath::uint64 cyclesPerSecond()					{return _frequency;}
	inline GMath::uint64 lastCyclesStamp() const					{return _cycles;}
	inline GMath::GTime deltaTime(const GHighResTimer& otherTimer)	{return otherTimer._timeElapsed-_timeElapsed;}
	inline double deltaSeconds(const GHighResTimer& otherTimer)		{return deltaTime(otherTimer).inSeconds();}
	inline float  deltaSecondsF(const GHighResTimer& otherTimer)	{return static_cast<float>(deltaTime(otherTimer).inSeconds());}
	inline GMath::GTime timeElapsed() const							{return _timeElapsed;}

private:
	//convert seconds in number of CPU cycles
	static inline  GMath::uint64 secondsToCycles(double seconds)	
	{
		return static_cast<GMath::uint64>( seconds*_frequency);
	}
	// WARNING: Dangerous -- only use to convert small number of cycles
	static inline  GMath::GTime	cyclesToTime(GMath::uint64 ncycles)
	{
		double sec = static_cast<double>(ncycles)/_frequency;
		return GMath::GTime(sec);
	}
	// WARNING: Dangerous -- only use to convert small number of cycles
	static inline double cyclesToSeconds(GMath::uint64 ncycles)
	{
		return static_cast<double>(ncycles)/_frequency;
	}

			GMath::uint64	_cycles;
			GMath::GTime	_timeElapsed;
	static	GMath::uint64	_frequency;
			GMath::Real		_scale;
			bool			_isPaused;

};


#endif // GHIGHRESTIMER_H

