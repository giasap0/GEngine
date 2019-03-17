#pragma once
#ifndef GLOGGER_H
#define GLOGGER_H
#include <GPhysic.h>
#include <fstream>

class GLogger
{
public:
	GLogger()					{ _pStringPlaneLog = nullptr; }
	virtual ~GLogger()
	{
		if (_pStringPlaneLog != nullptr) {
			_pStringPlaneLog->clear();
			delete _pStringPlaneLog;
			_pStringPlaneLog = nullptr;
		}
	}

	void start();
	void logAirplaneStatus(float dt, const GPhysics::GAirplaneStatus& status);
	void stop();

private:
	static const float _deltaTimeToLog;
	float _timeElapsed;
	float _sessionTime;
	GString * _pStringPlaneLog;
	std::ofstream _planeLogs;
private:
	//prevent copy
	GLogger(const GLogger&);
	GLogger& operator= (const GLogger&);
};

#endif			//GLOGGER_H