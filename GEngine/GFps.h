#pragma once
#ifndef GFPS_H
#define GFPS_H

#include <GMathCore.h>
#include <GTime.h>

using GMath::Int32;

class GFps
{
public:
	inline GFps(void)					{ _fps = _count = 0; _elapsedTime=0.0f;}
	~GFps(void)							{}

	inline void reset()					{ _fps = _count = 0;_elapsedTime=0.0f;}
	inline Int32 getFps()				{return _fps;}
	inline void update(GMath::GTime dt)
	{
		_count++;
		_elapsedTime+=dt.inSeconds();
		if( _elapsedTime>=1.0f)
		{
			_fps = _count;
			_count=0;
			_elapsedTime=0;
		}
	}

private:
	double _elapsedTime;
	Int32 _fps, _count;

	//prevent copy
	GFps(const GFps&);
	GFps& operator=(const GFps&);
};

#endif	//GFPS_H
