#pragma once
#ifndef GGAME_ENUMS_H
#define GGAME_ENUMS_H

#include "GGameCoreIncludes.h"

/*****************************************************
**		   Min			 Max		 Type		Bit	**
**			0			 +255		uint8_t		8	**
**		  -128		     +127		int8_t		8	**
**			0			+65,535		uint16_t	16	**	
**		-32,678			+32,767		int16_t		16	**	
**			0		+4,294,967,295	uint32_t	32	**
**	-2,147,483,648	+2,147,483,647	int32_t		32	**		//==int
******************************************************/

//-------------------------------------------------------------------------
// Typedefs
//-------------------------------------------------------------------------
typedef Int32 GObjIDType;
typedef Int16 GItemLevelType;
typedef Int16 GCharacterLevelType;

//-------------------------------------------------------------------------
// Enums
//-------------------------------------------------------------------------
namespace GGameEnums
{
enum class GItemType : uint8
{
	Invalid = 0, Armor = 1,	Weapon = 2, Consumable = 3
};

enum class GArmorType : uint8
{
	Invalid = 0, Cloth = 1, Leather = 2, Mail = 3, Plate = 4
};
enum class GArmorSlots : uint8
{
	Invalid = 0, Head = 1, Shoulder, Chest, Leg
};

enum class GWeaponType : uint8
{
	Invalid = 0, Sword = 1, Axe, Mace, Knife,
	Shield = 51,
	GreatSword = 101, Axe2H, Mace2H, Polearm,
	Bow = 151, ThrowingKnife
};

enum class GWeaponSlot : uint8
{
	Invalid = 0, MainHand = 1, OffHand=50, DualWield=100, Ranged=150
};

enum class GWearingSlot : uint8
{
	Invalid = 0, Helm=1, Shoulder, Back, Chest, Gloves, Belt, Pants, Shoes,
	MainHand=50, OffHand=51, Ranged=150,
};

enum class GDamageType : uint8
{
	Invalid = 0, Melee=1, Ranged, Cold , Heat, Electricity , Radiation 
};

enum class GBodyParts : uint8
{
	Invalid = 0, Head = 1, Chest, ArmL, ArmR, LegL, LegR
};

enum class GOrgansParts : uint8
{
	Invalid = 0, Heart=1, Liver, Lungs, kidneys
};



}	//EO namespace GGameEnums

#endif		//GGAME_ENUMS_H