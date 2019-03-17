#pragma once
#ifndef GCHARACTER_H
#define GCHARACTER_H

#include "GInventory.h"
#include "GDamage.h"


class GCharacter :	public GGameObj
{
protected:
	class Body;
	class Organs
	{
		friend class Body;
	public:
		Organs()												{_heart=_liver= _kidneys=_lungs=_heartMAX=_liverMAX= _kidneysMAX=_lungsMAX= 1.0f;}
		~Organs()												{}

		bool initialize( Int32 charID);

		void addHp( GOrgansParts part, Real amountToAdd);
		void damage(GOrgansParts part, float dmg);
		float hp(GOrgansParts part) const;

		inline void reset()										{_heart=_heartMAX; _liver=_liverMAX; _kidneys= _kidneysMAX; _lungs=_lungsMAX;}
		inline void reset(Real percentOfHp)						{_heart=_heartMAX*percentOfHp; _liver=_liverMAX*percentOfHp; _kidneys= _kidneysMAX*percentOfHp; _lungs=_lungsMAX*percentOfHp;}
		inline void healAll(Real amountToAdd)					{_heart+= amountToAdd; _liver+= amountToAdd; _kidneys+= amountToAdd; _lungs+= amountToAdd;	checkAll();}

	private:
		void checkAll();
		Real _heart, _liver, _lungs, _kidneys;
		Real _heartMAX, _liverMAX, _lungsMAX, _kidneysMAX;

		Organs(const Organs& o);
		Organs& operator=(const Organs& o);
	}; //EO class Organs
	
	class Body
	{
	public:
		Body();
		~Body()															{}

		bool initialize(Int32 charID);

		void reset();
		void reset(Real percentOfHp);

		void healAll(Real amountToAdd);
		Real damage(GBodyParts part, Real dmg);
		void healPart(GBodyParts part, Real amountToAdd);
		Real hp(GBodyParts part) const;

		inline void healOrgan( GOrgansParts part, Real amountToAdd)		{_organs.addHp(part, amountToAdd);}
		inline void damageOrgan( GOrgansParts part, Real dmg)			{_organs.damage(part, dmg);}
		inline Real organHP( GOrgansParts part) const					{return _organs.hp(part);}

		inline bool canRun() const										{if( isNull(_legL) || isNull(_legR) ) return false; return true;}
		inline bool canWalk() const										{if( isNull(_legL) && isNull(_legR) ) return false; return true;}
		inline bool canUseLeftArm() const								{return !isNull( _armL);}
		inline bool canUseRightArm() const								{return !isNull( _armR);}
		inline bool isDead() const
		{
			if( isNull(_head) || isNull(_organs._heart) || isNull(_organs._kidneys ) || isNull(_organs._liver ) || isNull(_organs._lungs) )
				return true;
			return false;
		}

	private:
		void checkAll();
		Body(const Body& b);
		Body& operator=(const Body& b);

		Organs _organs;
		Real _head, _chest, _armL, _armR, _legL, _legR;
		Real _headMAX, _chestMAX, _armMAX, _legMAX;
	}; //EO class Body

public:
	GCharacter(void)	: _level(-1)	{}
	virtual ~GCharacter(void)			{}

	virtual bool initialize(GObjIDType charID) = 0;

	virtual void getDamage( GBodyParts partToDmg, const GDamage& dmg)		{_body.damage( partToDmg,dmg.amount() );}
	virtual void getDamage( GOrgansParts partToDmg, const GDamage& dmg)		{_body.damageOrgan( partToDmg,dmg.amount() );}

	inline void getHeal(GBodyParts part, Real amountToAdd)					{_body.healPart(part,amountToAdd);}
	inline void getHeal( GOrgansParts part, Real amountToAdd)				{_body.healOrgan(part, amountToAdd);}
	inline void healAll(Real amountToAdd)									{_body.healAll(amountToAdd);}
	inline void revive(Real percentOfHp=0.3f)								{if(!isDead() ) return;_body.reset(percentOfHp);}
	inline void reviveFull()												{if(!isDead() ) return;_body.reset();}

	inline bool isDead() const												{return _body.isDead();}
	inline bool canRun() const												{return _body.canRun();}
	inline bool canWalk() const												{return _body.canWalk();}
	inline bool canUseLeftArm() const										{return _body.canUseLeftArm();}
	inline bool canUseRightArm() const										{return _body.canUseRightArm();}
	inline bool canWear( GArmorType t) const								{if(t<=_typeCanWear) return true; return false;}

	inline Real hp( GBodyParts part) const									{return _body.hp(part);}
	inline Real hp( GOrgansParts part ) const								{return _body.organHP(part);}

	inline GString					name() const							{return _name;}
	inline GCharacterLevelType		level() const							{return _level;}
	inline GArmorType				armorType() const						{return _typeCanWear;}
	inline GInventory*				inventory()								{return &_inventory;}
	inline const GInventory*		inventory() const						{return &_inventory;}
	inline GChar_WearingSlots*		wearing()								{return &_wearing;}
	inline const GChar_WearingSlots* wearing() const						{return &_wearing;}
	inline GMatrix4x4&				transformation()						{return _transformation;}
	inline const GMatrix4x4&		transformation() const					{return _transformation;}

protected:
	//components

	Body _body;
	GInventory _inventory;
	GChar_WearingSlots _wearing;

	//properties

	GMatrix4x4 _transformation;
	GString _name;
	GCharacterLevelType _level;
	GArmorType _typeCanWear;
private:
	GCharacter(const GCharacter&);
	GCharacter& operator=(const GCharacter&);
};	//EO class GCharacter


#endif //EO GCHARACTER_H