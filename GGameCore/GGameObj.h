#pragma once
#ifndef GGAMEOBJECT_H
#define GGAMEOBJECT_H

#include "GGameEnums.h"

using namespace GGameEnums;

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//class GGameObj (abstract) : every game object inherit from this class. Every game object(also chars) will have an unique ID, we'll use this one to initialize from file/db
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
class GGameObj
{
public:
	GGameObj(void)				: _objID(-1)			{}
	virtual ~GGameObj(void)								{}

	virtual bool initialize(GObjIDType objID) = 0;

	inline GObjIDType objID() const						{return _objID;}

protected:
	GObjIDType _objID;
};

//-------------------------------------------------------------------------
//class GItem (abstract) : every item in the game inherit from this class
//-------------------------------------------------------------------------
class GItem : public GGameObj
{
public:
	GItem ( GItemType t = GItemType::Invalid) : GGameObj(), _iName()	{_iType = t; _iLevel=-1;}
	virtual ~GItem()							{}

	virtual bool initialize(int) =0;

	inline GItemLevelType itemLevel() const					{return _iLevel;}
	inline GItemType	 itemType() const					{return _iType;}
	inline GString		 name() const						{return _iName;}

protected:
	GString _iName;
	GItemLevelType _iLevel;
	GItemType _iType;
};

#endif		//EO GGAMEOBJECT_H