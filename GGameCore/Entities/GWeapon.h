#pragma once
#ifndef GWEAPON_H
#define GWEAPON_H

#include "GGameObj.h"

class GWeapon : public GItem
{
public:
	GWeapon( GWeaponType t = GWeaponType::Invalid, GWeaponSlot s = GWeaponSlot::Invalid );
	virtual ~GWeapon()								{}

	virtual bool initialize(GObjIDType itemID);

	inline GWeaponType type() const					{return _wType;}
	inline GWeaponSlot slot() const					{return _wSlot;}
	inline Real meleeDmg() const					{return _meleeDmg;}
	inline Real magicDmg() const					{return _magicDmg;}
	inline Real minRange() const					{return _minRange;}
	inline Real maxRange() const					{return _maxRange;}

private:
	Real _meleeDmg;
	Real _magicDmg;
	Real _maxRange;
	Real _minRange;
	GWeaponType _wType;
	GWeaponSlot _wSlot;
};

#endif	//EO GWEAPON_H