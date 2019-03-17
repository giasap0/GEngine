#pragma once
#ifndef GDAMAGE_H
#define GDAMAGE_H

#include "GGameEnums.h"
using GGameEnums::GDamageType;
using GMath::clamp;

class GDamage
{
public:
	GDamage()																		{_amount=_hitChance=.0f; _type=GDamageType::Invalid;}
	GDamage(const GDamage& d)														{_amount=d._amount; _hitChance=d._hitChance; _type=d._type;}
	~GDamage()																		{}

	inline Real amount() const														{return _amount;}
	inline Real hitChance() const													{return _hitChance;}
	inline GDamageType type() const													{return _type;}

	inline GDamage& operator=(const GDamage& d)										{_amount=d._amount; _hitChance=d._hitChance; _type=d._type; return *this;}
	inline GDamage& operator=(Real dmgValue)										{_amount = dmgValue; if(_amount<=.0f) _amount=.0f; return *this;}
	inline void setValues( Real value, Real chanceToHit, GDamageType t)				{_amount=value; _hitChance=chanceToHit; _type=t; if(_amount<=.0f) _amount=.0f; clamp(_hitChance,.0f,1);}
	inline void setHitChance(Real chanceToHit)										{_hitChance = chanceToHit; clamp(_hitChance,.0f,1); }
	inline void setType(GDamageType newType)										{_type = newType;}

	inline GDamage operator+(Real add) const										{GDamage d(*this); d._amount +=add; return d;}
	inline GDamage& operator+=(Real add)											{_amount+= add; return *this;}
	inline GDamage operator-(Real sub) const										{GDamage d(*this); d._amount -=sub; if(d._amount<=.0f) d._amount=.0f; return d;}
	inline GDamage& operator-=(Real sub)											{_amount-= sub; if(_amount<=.0f) _amount=.0f; return *this;}
	inline GDamage operator*(Real mul) const										{GDamage d(*this); d._amount *=mul; if(d._amount<=.0f) d._amount=.0f; return d;}
	inline GDamage& operator*=(Real mul)											{_amount*= mul; if(_amount<=.0f) _amount=.0f; return *this;}
	inline GDamage operator/(Real div) const										{GDamage d(*this); d._amount /=div; if(d._amount<=.0f) d._amount=.0f; return d;}
	inline GDamage& operator/=(Real div)											{_amount/= div; if(_amount<=.0f) _amount=.0f; return *this;}

	inline bool isValid() const														{return (_type != GDamageType::Invalid && _amount>= 0.0f );}
	//value == 0
	inline bool isNull() const														{return GMath::isNull( _amount ); }
	bool operator==(const GDamage& d) const;
	inline bool operator==(Real dmgValue) const										{return _amount==dmgValue;}
	inline bool operator>(Real dmgValue) const										{return _amount>dmgValue;}
	inline bool operator>=(Real dmgValue) const										{return _amount>=dmgValue;}
	inline bool operator<(Real dmgValue) const										{return _amount<dmgValue;}
	inline bool operator<=(Real dmgValue) const										{return _amount<=dmgValue;}

private:
	Real _amount;
	Real _hitChance;
	GDamageType _type;
};

#endif		//EO GDamage