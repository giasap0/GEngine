#pragma once
#ifndef GARMOR_H
#define GARMOR_H

#include "GGameObj.h"

class GArmor :	public GItem
{
public:
	GArmor( GArmorType at = GArmorType::Invalid, GArmorSlots as = GArmorSlots::Invalid);
	virtual ~GArmor(void);

	virtual bool initialize(GObjIDType itemID);

	inline void repairArmor()					{_currentArmorValue=_maxValue;}
	inline Real armorValue() const				{return _currentArmorValue;}
	inline GArmorSlots slot() const				{return _aSlot;}
	inline GArmorType type() const				{return _aType;}
	inline bool isValid() const					{return (_aSlot != GArmorSlots::Invalid);}

	//if(dmg>0) return (dmg-armor); return 0
	inline Real getDmg(Real dmg)
	{
		if(dmg<=.0f)
			return .0f;
		if( dmg <= _currentArmorValue)
		{
			_currentArmorValue-=dmg;
			return .0f;
		}
		else
		{
			Real delta = dmg- _currentArmorValue;
			_currentArmorValue = .0f;
			return delta;
		}
	}


private:
	Real _currentArmorValue;
	Real _maxValue;
	GArmorType _aType;
	GArmorSlots _aSlot;
};

#endif		//EO GARMOR_H