#pragma once
#ifndef GINVENTORY_H
#define GINVENTORY_H

#include "GArmor.h"
#include "GWeapon.h"

class GInventory;

//-------------------------------------------------------------
//class WearingSlots : a container for what a character is wearing
//-------------------------------------------------------------
class GChar_WearingSlots
{
public:
	GChar_WearingSlots();
	virtual ~GChar_WearingSlots();

	bool initialize(GObjIDType charID);

	//could be nullptr!!
	inline GWeapon* getWeapon( GWeaponSlot slot ) const;
	//could be nullptr!!
			GArmor* getArmor( GArmorSlots slot);
	const	GArmor* getArmor( GArmorSlots slot) const;
			GItem* getItem( GWearingSlot slot );
	const	GItem* getItem( GWearingSlot slot ) const;
	void moveItemToInventory( GWearingSlot slot );

private:
	GArmor* _armor[4];						//0=head, 1=chest, 2=houlder, 3=leg
	GWeapon* _weaponMH;
	GWeapon* _weaponOH;
	GWeapon* _weaponRanged;
	GItem* _back, *_gloves, *_belt, *_shoes;
	GInventory* _inventoryPtr;		//Character own inventory
};


//--------------------------------------------------------------
//class GInventory : a container for all items a character owns
//--------------------------------------------------------------
class GInventory
{
public:
	GInventory()			: _currentNumberOfItems(0), _items(0)		{}
	virtual ~GInventory();

	bool initialize(GObjIDType charID);

	inline uint16 size() const							{return _items.size();}
	inline uint numberOfItems() const					{return _currentNumberOfItems;}

	//return false only if inventory is full
	bool addItem( GItem* itemPtr );
	inline const GItem* getItem(uint position) const	{if( position>= _items.size()) return nullptr; return _items[position];}
	inline		 GItem* getItem(uint position)			{if( position>= _items.size()) return nullptr; return _items[position];}
	inline void deleteItem( uint position)				{if( position>= _items.size()) return; if(_items[position] != nullptr){ --_currentNumberOfItems; delete _items[position]; _items[position]=nullptr;}}

	inline bool isEmpty() const							{return  (_currentNumberOfItems==0);}
	inline bool isFull() const							{return  (_currentNumberOfItems >= _items.size());}

private:
	uint16 _currentNumberOfItems;
	GVector< GItem*> _items;
};

#endif	//EO GINVENTORY_H