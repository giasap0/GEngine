#include "GPhysic.h"

using namespace GPhysics;
using namespace GMath;

bool GPhysic::initialize()
{
	GAirplaneBuildInfo build;
	bool ret = GAirplaneLoader::loadBuildingInfo(build);
	if (!ret)
		return false;
	_pAirPlane = new GAirplane();
	_pAirPlane->initialize(build);
	return true;
}

void GPhysic::shutDown()
{
	if (_pAirPlane != nullptr)
	{
		delete _pAirPlane;
		_pAirPlane = nullptr;
	}
}

void GPhysic::updateState(Real dt, const GPlaneCommand& planeCommand)
{
	_pAirPlane->updateState(dt, planeCommand);
}
