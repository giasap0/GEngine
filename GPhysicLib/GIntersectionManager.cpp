#include "GIntersectionManager.h"


GBoxManager::GBoxManager()
{
	//larger than any world coordinate of game
	Real invalid = Constants::realMax - 10.0f;
	for(int i=0, j=0; i < PhysicSystemConstants::NUM_BOXES; ++i)
	{
		_xEndpoints[j].set(i, invalid, true);
		_yEndpoints[j].set(i, invalid, true);
		_zEndpoints[j].set(i, invalid, true);
		++j;
		invalid += 1.0f;

		_xEndpoints[j].set(i, invalid, true);
		_yEndpoints[j].set(i, invalid, true);
		_zEndpoints[j].set(i, invalid, true);
		++j;
		invalid += 1.0f;
	}

	for(int j=0; j < PhysicSystemConstants::NUM_ENDPOINTS; ++j)
	{
		_xLookUp[ _xEndpoints[j].lookup()] = j;
		_yLookUp[ _yEndpoints[j].lookup()] = j;
		_zLookUp[ _zEndpoints[j].lookup()] = j;
	}

	_numMoveQueue = _numEvents = 0;
	memset( _events,0, PhysicSystemConstants::MAX_EVENTS*sizeof(Int32) );
}

bool GBoxManager::moveBoxes(const GShapes::GBox* boxes)
{
	for(int i=0; i< _numMoveQueue; ++i)
	{
		move(_moveQueue[i], boxes[_moveQueue[i] ]);
	}
	bool hasMoved = (_numMoveQueue>0);
	_numMoveQueue=0;
	return hasMoved;									//if nothing has moved, there are no physics to apply.
}

void GBoxManager::move(Int32 i, const GShapes::GBox& box)
{
	//_xEndpoints[ _xLookUp[2*i] ].value() = box.center().x() - 
}
