#include "GLogger.h"
#include <Windows.h>

//limit logs to only 50 times per seconds
const float GLogger::_deltaTimeToLog = 1.000f/50.0f;

void GLogger::start()
{
	_sessionTime = .0f;
	_timeElapsed = .0f;
	GString fileName = "logs/Glog_airplane_status_";
	SYSTEMTIME st;
	GetLocalTime(&st);
	fileName += GString::number(st.wYear) + GString("_") + GString::number(st.wMonth) + GString("_") + GString::number(st.wDay);
	fileName += "_";
	fileName += GTime::currentTime().toString("_");
	fileName += ".txt";
	_planeLogs.clear();
	_planeLogs.open(fileName.const_data(), std::ios::out);
	_planeLogs << "axis:\n";
	_planeLogs << "^    z, pilot's foot-head\n";
	_planeLogs << "|\n";
	_planeLogs << "|\n";
	_planeLogs << "(x)---------> y , y // wings \n";
	_planeLogs << "Start " << GTime::currentTime().toString().const_data() << ",[International System of Units]\n\n";
	_planeLogs << "time elapsed [s] , position [m],,, speed [m/s],,, acceleration [m/s^2],,,alfa [rad],beta [rad],roll [rad],pitch [rad],yaw [rad], angular_speed [rad/s],,, angular_acceleration [rad/s^2],,, Weight [N],,,Thrust [N],,, Drag [N],,, Lift [N],,,Y [N],,,Mx [Nm],,,My [Nm],,,Mz [Nm],,,aileron [rad],elevator [rad],rudder [rad]\n";

	if (_pStringPlaneLog != nullptr)
	{
		_pStringPlaneLog->clear();
		delete _pStringPlaneLog;
		_pStringPlaneLog = nullptr;
	}
	_pStringPlaneLog = new GString();
}

void GLogger::stop()
{
	_planeLogs.close();
	if (_pStringPlaneLog != nullptr)
	{
		_pStringPlaneLog->clear();
		delete _pStringPlaneLog;
		_pStringPlaneLog = nullptr;
	}
	_sessionTime = .0f;
	_timeElapsed = .0f;
}

void GLogger::logAirplaneStatus(float dt, const GPhysics::GAirplaneStatus& status)
{
	_sessionTime += dt;
	_timeElapsed += dt;
	if (_timeElapsed < _deltaTimeToLog)
		return;
	if (_pStringPlaneLog == nullptr)
		_pStringPlaneLog = new GString();
	else
		_pStringPlaneLog->clear();

	_pStringPlaneLog->append(GString::number(_sessionTime) + ",");
	_pStringPlaneLog->append(status.position.translationVector().toSting_onlyValues() + ",");
	_pStringPlaneLog->append(status.v.toSting_onlyValues() + ",");
	_pStringPlaneLog->append(status.a.toSting_onlyValues() + ",");
	_pStringPlaneLog->append(status.alfa.toString() + "," + status.beta.toString() + ",");
	_pStringPlaneLog->append(status.aroundX.toString() + "," + status.aroundY.toString() + ","+status.aroundZ.toString() + ",");
	_pStringPlaneLog->append(status.w.toSting_onlyValues() + ",");
	_pStringPlaneLog->append(status.w_dot.toSting_onlyValues() + ",");
	_pStringPlaneLog->append(status.weight.toSting_onlyValues() + ",");
	_pStringPlaneLog->append(status.thrust.toSting_onlyValues() + ",");
	_pStringPlaneLog->append(status.drag.toSting_onlyValues() + ",");
	_pStringPlaneLog->append(status.lift.toSting_onlyValues() + ",");
	_pStringPlaneLog->append(status.yAereodyn.toSting_onlyValues() + ",");
	_pStringPlaneLog->append(status.mx.toSting_onlyValues() + ",");
	_pStringPlaneLog->append(status.my.toSting_onlyValues() + ",");
	_pStringPlaneLog->append(status.mz.toSting_onlyValues() + ",");
	_pStringPlaneLog->append(status.commands.roll.toString() + "," + status.commands.pitch.toString() + "," + status.commands.yaw.toString() + ",");
	_planeLogs << _pStringPlaneLog->const_data() << "\n";

	_timeElapsed = .0f;
}

/*
_planeLogs << "timestamp , position,,, speed,,, acceleration,,,
				alfa,beta,pitch,yaw,roll, angular_speed,,, angular_acceleration,,, 
				Weight,,,Thrust ,,, 
				Drag,,, Lift,,,Y,,,Mx,,,My,,,Mz\n";
struct GAirplaneStatus
{
GAirplaneStatus()				{ position = GMatrix4x4::Identity();  }
//position / orientation
GMatrix4x4 position;
//speed
GVector3 v;
//angular speed
GVector3 w;
//acceleration
GVector3 a;
//angular acceleration
GVector3 w_dot;

//yAereodyn is the lateral-aereodynamic force.
GVector3 lift, drag, yAereodyn;
GVector3 thrust, weight;
GVector3 mx, my, mz;
GMath::Radians aroundX, aroundY, aroundZ;
GMath::Radians alfa, beta;
};
*/