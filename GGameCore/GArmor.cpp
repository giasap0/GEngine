#include "GArmor.h"


GArmor::GArmor( GArmorType at, GArmorSlots as)	: _currentArmorValue(-1) , _maxValue(-1)
{
	_aType= at; _aSlot=as;
}

GArmor::~GArmor(void)
{
}

bool GArmor::initialize(GObjIDType ItemID)
{
	return false;
}
