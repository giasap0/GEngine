#pragma once
#ifndef GPHYSIC_ENUMS_H
#define GPHYSIC_ENUMS_H

#include "GPhysicIncludes.h"

namespace  PhysicSystemConstants
{
	enum
	{
		//number of boxes in system
		NUM_BOXES = 1000,
		//number of bits per index
		LG2_NUM_BOXES = 10,					//==log2( num_boxes )
		//number of endpoints in system
		NUM_ENDPOINTS = 2000,
		//bit mask for maximum endpoint
		TYPE_MAXIMUM = 0x80000000,
		//maximum number of events for one physics frame
		MAX_EVENTS = 20,
		//bit mask for insert event during update
		INSERT_EVENT = 0x80000000,
		//1-value bits for low-order lg2_num_boxes bits. Else 0-value bits
		MASK_INDEX0 = 0x000003FF,
		//0-value bits for low-order lg2_num_boxes bits. 1-value for the next lg2_num_boxes bits. 0-value others
		MASK_INDEX1 = 0x000FFC00
	};
}


#endif		//ENUMS_H