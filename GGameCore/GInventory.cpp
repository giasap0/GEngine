#include "GInventory.h"

//--------------------------------------------------------------------------------------------
//************************************ GChar_WearingSlots ************************************
//--------------------------------------------------------------------------------------------

GChar_WearingSlots::GChar_WearingSlots() : _weaponMH(nullptr), _weaponOH(nullptr), _weaponRanged(nullptr),
										_back(nullptr), _gloves(nullptr), _belt(nullptr), _shoes(nullptr),	_inventoryPtr(nullptr)
{
	for(int i=0; i<4; i++)
	{
		_armor[i]= nullptr;
	}
}

GChar_WearingSlots::~GChar_WearingSlots()
{
	_inventoryPtr = nullptr;
	SAFE_DELETE( _shoes );
	SAFE_DELETE(_belt );
	SAFE_DELETE( _gloves );
	SAFE_DELETE( _back );
	SAFE_DELETE( _weaponRanged );
	SAFE_DELETE( _weaponOH );
	SAFE_DELETE( _weaponMH );
	
	for(int i=0; i<4; i++)
	{
		SAFE_DELETE( _armor[i] );
	}
}

bool GChar_WearingSlots::initialize(GObjIDType charID)
{
	return false;
}

GWeapon* GChar_WearingSlots::getWeapon( GWeaponSlot slot ) const
{
	if( slot == GWeaponSlot::Invalid)
		return nullptr;
	if(slot == GWeaponSlot::OffHand)
		return _weaponOH;
	if( _weaponMH == nullptr )
		return nullptr;
	switch (slot)
	{
	case GGameEnums::GWeaponSlot::MainHand: if(_weaponMH->slot() == GWeaponSlot::MainHand ) return _weaponMH;
		break;
	case GGameEnums::GWeaponSlot::DualWield: if( _weaponMH->slot() == GWeaponSlot::DualWield ) return _weaponMH;
		break;
	case GGameEnums::GWeaponSlot::Ranged: if( _weaponMH->slot() == GWeaponSlot::Ranged ) return _weaponRanged;
		break;
	default: return nullptr;
		break;
	}
	return nullptr;
}

const GArmor* GChar_WearingSlots::getArmor( GArmorSlots slot) const
{
	switch (slot)
	{
	case GGameEnums::GArmorSlots::Invalid: return nullptr;
		break;
	case GGameEnums::GArmorSlots::Head: return _armor[0];
		break;
	case GGameEnums::GArmorSlots::Chest: return _armor[1];
		break;
	case GGameEnums::GArmorSlots::Shoulder:return _armor[2];
		break;
	case GGameEnums::GArmorSlots::Leg:return _armor[3];
		break;
	default: return nullptr;
		break;
	}
	return nullptr;
}
GArmor* GChar_WearingSlots::getArmor( GArmorSlots slot)
{
	switch (slot)
	{
	case GGameEnums::GArmorSlots::Invalid: return nullptr;
		break;
	case GGameEnums::GArmorSlots::Head: return _armor[0];
		break;
	case GGameEnums::GArmorSlots::Chest: return _armor[1];
		break;
	case GGameEnums::GArmorSlots::Shoulder:return _armor[2];
		break;
	case GGameEnums::GArmorSlots::Leg:return _armor[3];
		break;
	default: return nullptr;
		break;
	}
	return nullptr;
}

const GItem* GChar_WearingSlots::getItem( GWearingSlot slot ) const
{
	switch (slot)
	{
	case GGameEnums::GWearingSlot::Invalid: return nullptr;
		break;
	case GGameEnums::GWearingSlot::Helm: return getArmor( GArmorSlots::Head );
		break;
	case GGameEnums::GWearingSlot::Shoulder: return getArmor( GArmorSlots::Shoulder );
		break;
	case GGameEnums::GWearingSlot::Back: return _back;
		break;
	case GGameEnums::GWearingSlot::Chest: return getArmor( GArmorSlots::Chest );
		break;
	case GGameEnums::GWearingSlot::Gloves: return _gloves;
		break;
	case GGameEnums::GWearingSlot::Belt: return _belt;
		break;
	case GGameEnums::GWearingSlot::Pants: return getArmor( GArmorSlots::Leg );
		break;
	case GGameEnums::GWearingSlot::Shoes: return _shoes;
		break;
	case GGameEnums::GWearingSlot::MainHand: return _weaponMH;
		break;
	case GGameEnums::GWearingSlot::OffHand: return _weaponOH;
		break;
	case GGameEnums::GWearingSlot::Ranged: return _weaponRanged;
		break;
	default:	return nullptr;
		break;
	}
	return nullptr;
}
GItem* GChar_WearingSlots::getItem( GWearingSlot slot )
{
	switch (slot)
	{
	case GGameEnums::GWearingSlot::Invalid: return nullptr;
		break;
	case GGameEnums::GWearingSlot::Helm: return getArmor( GArmorSlots::Head );
		break;
	case GGameEnums::GWearingSlot::Shoulder: return getArmor( GArmorSlots::Shoulder );
		break;
	case GGameEnums::GWearingSlot::Back: return _back;
		break;
	case GGameEnums::GWearingSlot::Chest: return getArmor( GArmorSlots::Chest );
		break;
	case GGameEnums::GWearingSlot::Gloves: return _gloves;
		break;
	case GGameEnums::GWearingSlot::Belt: return _belt;
		break;
	case GGameEnums::GWearingSlot::Pants: return getArmor( GArmorSlots::Leg );
		break;
	case GGameEnums::GWearingSlot::Shoes: return _shoes;
		break;
	case GGameEnums::GWearingSlot::MainHand: return _weaponMH;
		break;
	case GGameEnums::GWearingSlot::OffHand: return _weaponOH;
		break;
	case GGameEnums::GWearingSlot::Ranged: return _weaponRanged;
		break;
	default:	return nullptr;
		break;
	}
	return nullptr;
}

void GChar_WearingSlots::moveItemToInventory( GWearingSlot slot )
{
	if (_inventoryPtr== nullptr)
		return;
	GItem* ptr = nullptr;
	
	switch (slot)
	{
	case GGameEnums::GWearingSlot::Invalid: return;
		break;
	case GGameEnums::GWearingSlot::Helm: ptr = _armor[0]; _armor[0]= nullptr;
		break;
	case GGameEnums::GWearingSlot::Shoulder: ptr = _armor[2]; _armor[2]=nullptr;
		break;
	case GGameEnums::GWearingSlot::Back: ptr = _back; _back = nullptr;
		break;
	case GGameEnums::GWearingSlot::Chest: ptr = _armor[1]; _armor[1]=nullptr;
		break;
	case GGameEnums::GWearingSlot::Gloves: ptr = _gloves; _gloves = nullptr;
		break;
	case GGameEnums::GWearingSlot::Belt: ptr = _belt; _belt = nullptr;
		break;
	case GGameEnums::GWearingSlot::Pants: ptr = _armor[3]; _armor[3] = nullptr;
		break;
	case GGameEnums::GWearingSlot::Shoes: ptr = _shoes; _shoes = nullptr;
		break;
	case GGameEnums::GWearingSlot::MainHand: ptr = _weaponMH; _weaponMH = nullptr;
		break;
	case GGameEnums::GWearingSlot::OffHand: ptr = _weaponOH; _weaponOH = nullptr;
		break;
	case GGameEnums::GWearingSlot::Ranged: ptr = _weaponRanged; _weaponRanged = nullptr;
		break;
	default: return;
		break;
	}
	_inventoryPtr->addItem( ptr );
}



//------------------------------------------------------------------------------------
//************************************ GInventory ************************************
//------------------------------------------------------------------------------------

GInventory::~GInventory(void)
{
	for( uint i=0; i<_items.size(); i++)
	{
		SAFE_DELETE( _items[i] );
	}
	_items.clear();
}


bool GInventory::initialize(GObjIDType charID)
{
	//TIP: check for _slotsNumber and resize _items to that number. Then sqeeze. I use vector size to know total slots number
	return false;
}

bool GInventory::addItem( GItem* itemPtr )
{
	if( isFull() )
		return false;
	for( uint i=0; i<_items.size(); i++)
	{
		if( _items[i] == nullptr )
		{
			_items[i]= itemPtr;
			++_currentNumberOfItems;
			return true;
		}
	}
	return false;
}
