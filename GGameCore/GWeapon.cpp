#include "GWeapon.h"


GWeapon::GWeapon( GWeaponType t, GWeaponSlot s ) : _meleeDmg(.0f), _magicDmg(.0f), _maxRange(1), _minRange(0)
{
	_wType= t;
	_wSlot = s;
}


bool GWeapon::initialize(GObjIDType itemID)
{
	return false;
}
