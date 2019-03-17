#include "GDamage.h"

using namespace GMath;

bool GDamage::operator==(const GDamage& d) const				// _amount==d._amount && _hitChance==d._hitChance && _type==d._type );
{
	return ( areEqual( _amount, d._amount) && areEqual( _hitChance, d._hitChance) && _type==d._type);
}