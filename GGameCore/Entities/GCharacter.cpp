#include "GCharacter.h"


//---------------------------------------------------------------------------------------------------------------------
//************************************************** GCharacter::Organs **************************************************
//---------------------------------------------------------------------------------------------------------------------

//void GCharacter::Organs::initialize( Int32 charID);

void GCharacter::Organs::addHp(GOrgansParts part, float amountToAdd)
{
	if( amountToAdd<=.0f)
		return;
	switch (part)
	{
		case GOrgansParts::Invalid: return;
			break;
		case GOrgansParts::Heart: _heart+= amountToAdd; clampMax(_heart, _heartMAX);
			break;
		case GOrgansParts::Liver: _liver+= amountToAdd; clampMax( _liver, _liverMAX);
			break;
		case GOrgansParts::kidneys: _kidneys+= amountToAdd; clampMax( _kidneys, _kidneysMAX);
			break;
		case GOrgansParts::Lungs: _lungs+= amountToAdd; clampMax(_lungs, _lungsMAX);
			break;
		default:
			break;
	}
}

void GCharacter::Organs::damage(GOrgansParts part, float dmg)
{
	if(dmg<=.0f)
		return;
	switch (part)
	{
		case GOrgansParts::Invalid: return;
			break;
		case GOrgansParts::Heart: _heart-=dmg; clampMin(_heart, .0f); return;
			break;
		case GOrgansParts::Liver: _liver-=dmg; clampMin(_liver,.0f); return;
			break;
		case GOrgansParts::kidneys: _kidneys-= dmg; clampMin(_kidneys, 0.0f); return;
			break;
		case GOrgansParts::Lungs: _lungs-=dmg; clampMin(_lungs,.0f); return;
			break;
		default: return;
			break;
		}
}

float GCharacter::Organs::hp( GOrgansParts part ) const
{
	switch (part)
	{
		case GOrgansParts::Invalid: return -1.0f;
			break;
		case GOrgansParts::Heart: return _heart;
			break;
		case GOrgansParts::Liver: return _liver;
			break;
		case GOrgansParts::kidneys: return _kidneys;
			break;
		case GOrgansParts::Lungs: return _lungs;
			break;
		default: return -1.0f;
			break;
	}
	return -1.0f;
}

void GCharacter::Organs::checkAll()
{
	if(_heart>_heartMAX)
		_heart=_heartMAX;
	if(_liver>_liverMAX)
		_liver=_liverMAX;
	if(_kidneys> _kidneysMAX)
		_kidneys= _kidneysMAX;
	if(_lungs>_lungsMAX)
		_lungs=_lungsMAX;
}

//---------------------------------------------------------------------------------------------------------------------
//************************************************** GCharacter::Body **************************************************
//---------------------------------------------------------------------------------------------------------------------

void GCharacter::Body::reset( float percentOfHp)
{		
	_head= _headMAX*percentOfHp; _chest=_chestMAX*percentOfHp;
	_armL=_armR=_armMAX*percentOfHp;
	_legL=_legR=_legMAX*percentOfHp;
	_organs.reset(percentOfHp);
}
void GCharacter::Body::reset()
{
	_head= _headMAX; _chest=_chestMAX;
	_armL=_armR=_armMAX;
	_legL=_legR=_legMAX;
	_organs.reset();
}

void GCharacter::Body::healAll(float amountToAdd)
{
	_head+=amountToAdd;_chest+= amountToAdd;
	_armL+= amountToAdd;_armR+= amountToAdd;
	_legL+= amountToAdd;_legR+= amountToAdd;
	checkAll();
	_organs.healAll(amountToAdd);
}


void GCharacter::Body::healPart(GBodyParts part, float amountToAdd)
{
	if( amountToAdd <= .0f)
		return;
	switch (part)
	{
	case GBodyParts::Invalid: return;
		break;
	case GBodyParts::Head: _head+= amountToAdd; if(_head>_headMAX) _head=_headMAX;
		break;
	case GBodyParts::Chest: _chest+= amountToAdd; if(_chest>_chestMAX) _chest=_chestMAX;
		break;
	case GBodyParts::ArmL: _armL+= amountToAdd; if( _armL>_armMAX) _armL=_armMAX;
		break;
	case GBodyParts::ArmR: _armR+= amountToAdd; if( _armR>_armMAX)_armR=_armMAX;
		break;
	case GBodyParts::LegL:_legL+= amountToAdd; if(_legL>_legMAX) _legL=_legMAX;
		break;
	case GBodyParts::LegR:_legR+= amountToAdd; if(_legR>_legMAX)	_legR=_legMAX;
		break;
	default:
		break;
	}
};

Real GCharacter::Body::hp(GBodyParts part) const
{
	switch (part)
	{
	case GBodyParts::Invalid: return -1.0f;
		break;
	case GBodyParts::Head: return _head;
		break;
	case GBodyParts::Chest: return _chest;
		break;
	case GBodyParts::ArmL: return _armL;
		break;
	case GBodyParts::ArmR: return _armR;
		break;
	case GBodyParts::LegL: return _legL;
		break;
	case GBodyParts::LegR: return _legR;
		break;
	default: return -1.0f;
		break;
	}
	return -1.0f;
}

void GCharacter::Body::checkAll()
{
	if(_head>_headMAX)
		_head=_headMAX;
	if(_chest>_chestMAX)
		_chest=_chestMAX;
	if( _armL>_armMAX)
		_armL=_armMAX;
	if( _armR>_armMAX)
		_armR=_armMAX;
	if(_legL>_legMAX)
		_legL=_legMAX;
	if(_legR>_legMAX)
		_legR=_legMAX;
}

//-------------------------------------------------------------------------------------------------------------
//************************************************** GCharacter **************************************************
//-------------------------------------------------------------------------------------------------------------


//bool GCharacter::initialize(GObjIDType charID)	{}