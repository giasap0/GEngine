#pragma once
#ifndef GINTERSECTIONMANAGER_H
#define GINTERSECTIONMANAGER_H

#include "GPhysicEnums.h"
#include <GShapes.h>

class GEndPoint
{
public:
	GEndPoint()											{ _value = .0f; _index = 0; }

	Real value() const									{return _value;}
	Real& value()										{return _value;}
	Int32 type() const									{return ((_index & PhysicSystemConstants::TYPE_MAXIMUM) >> PhysicSystemConstants::LG2_NUM_BOXES);}
	Int32 index() const									{return _index & PhysicSystemConstants::TYPE_MAXIMUM;}
	Int32 lookup() const								{return ((2*( _index & PhysicSystemConstants::TYPE_MAXIMUM)) + ( (_index & PhysicSystemConstants::TYPE_MAXIMUM)>> PhysicSystemConstants::LG2_NUM_BOXES));}
	void set(Int32 i, Real value, bool isMinimum)
	{
		_value=value;
		_index = ( isMinimum ? i : (i | PhysicSystemConstants::TYPE_MAXIMUM) );
	}
	
	bool operator< (const GEndPoint& end) const
	{
		if( areEqual( _value, end._value) )
			return ( type() < end.type() );
		return (_value < end._value);
	}

private:
	//The high-order bit stores the type information. The 15 low-order bits store the actual box index.
	Int32 _index;
	Real _value;
};

class GPairIndices
{
public:
	GPairIndices(Int32 i0, Int32 i1)		{if(i0<i1){_i0=i0; _i1=i1;}else{_i0=i1; _i1=i0;} }

	Int32 i0() const				{return _i0;}
	Int32& i0()						{return _i0;}
	Int32 i1() const				{return _i1;}
	Int32& i1()						{return _i1;}
private:
	Int32 _i0, _i1;
};


class GBoxManager
{
public:
	GBoxManager();

	inline void insertMoveQueue( Int32 i)											{_moveQueue[ _numMoveQueue++] = i;}
	bool moveBoxes(const GShapes::GBox* boxes);
	void update();
	bool processEvents( GBSTree<GPairIndices>& overlaps);

private:
	void move(Int32 i, const GShapes::GBox& box);
	bool overlapping(Int32 i0, Int32 i1);
	bool overlapping( Int32 i0, Int32 i1, GEndPoint* endpoints, Int32* lookup);
	void update( GEndPoint* endpoints, Int32* lookup);

	GEndPoint _xEndpoints[PhysicSystemConstants::NUM_ENDPOINTS];
	GEndPoint _yEndpoints[PhysicSystemConstants::NUM_ENDPOINTS];
	GEndPoint _zEndpoints[PhysicSystemConstants::NUM_ENDPOINTS];
	Int32 _xLookUp[PhysicSystemConstants::NUM_ENDPOINTS];
	Int32 _yLookUp[PhysicSystemConstants::NUM_ENDPOINTS];
	Int32 _zLookUp[PhysicSystemConstants::NUM_ENDPOINTS];

	Int32 _numMoveQueue;
	Int32 _moveQueue[PhysicSystemConstants::NUM_BOXES];
	Int32 _numEvents;
	Int32 _events[PhysicSystemConstants::MAX_EVENTS];

	//prevent copy
	GBoxManager(GBoxManager&);
	GBoxManager& operator=(const GBoxManager&);
};



class GIntersectionManager
{
public:
	GIntersectionManager(void);
	~GIntersectionManager(void);
};

#endif		//GINTERSECTIONMANAGER_H