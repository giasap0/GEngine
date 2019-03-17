#pragma once
#ifndef GENGINE_CORE_H
#define GENGINE_CORE_H 

//--------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------
#include "GStringList.h"
#include "GWStringList.h"
#include "GStack.h"
#include "GTree.h"
#include "GBSTree.h"
#include "GHeap.h"
#include "GGraph.h"
#include "GWeightedGraph.h"

//--------------------------------------------------------------------------------------
// Generic Macros
//-------------------------------------------------------------------------------------

#define MAX_FILENAME_LENGTH 255

//it's for debug output
#define DBG_OUTPT( s )						 \
{											 \
   std::wostringstream os_;					 \
   os_ << s << "\n";						 \
   OutputDebugStringW( os_.str().c_str() );  \
}

//safe deleting a pointer
#define SAFE_DELETE( p )	\
{                           \
	if( p != nullptr)		\
	{						\
		delete (p);			\
		(p)= nullptr;		\
	}						\
}

//--------------------------------------------------------------------------------------
// Generic Functions
//-------------------------------------------------------------------------------------

//dynamic_cast in debug, else static_cast // return cast<Son*>(Father*)
template<class Father, class Son> inline Son* checked_cast(Father* fPtr)
{
	Son* ptr = nullptr;
#ifdef _DEBUG
	ptr = dynamic_cast<Son*>(fPtr);
	if( ptr == nullptr)
		DBG_OUTPT("checked_cast : dynamic_cast returning a nullPtr - cast is not valid. Father == "<< typeid(Father).name() <<" , Son == " << typeid(Son).name() );
#else
	ptr = static_cast<Son*>(fPtr);
#endif // DEBUG
	return ptr;
}

//--------------------------------------------------------------------------------------
// Error Handling
//-------------------------------------------------------------------------------------

//	0	<= uint8_t	 <=	+255

//unsigned int 8bit
//=0(success) other values (errors)
enum class GError: uint8_t
{
	//no errors
	Success		= 0,
	//Operation aborted
	Abort		= 1,
	//unspecified failure
	Fail		= 2,	
	//invalid arguments
	InvalidArgs	= 3,
	//failed to allocate memory
	OutOfMemory	= 4,		

	FailInitWindow = 25,

	DividedZero	 = 50,
	//request sqrt of a negative number
	SQRtnegative = 51		
};

inline bool GSucceeded(GError er)		{if(er==GError::Success) return true; else return false;}
inline bool GFailed   (GError er)		{if(er!=GError::Success) return true; else return false;}


inline GString GErrorString( GError er)
{
	GString str;
	switch (er)
	{
	case (GError::Success) : str="Success"; break;
	case (GError::Abort) : str="Operation Aborted"; break;
	case(GError::Fail) : str="Unspecified Failure"; break;
	case(GError::InvalidArgs) : str = "Invalid Arguments"; break;
	case(GError::OutOfMemory) : str = "Out Of Memory"; break;
	case(GError::FailInitWindow) : str = "Failed To Init A Window"; break;
	case(GError::DividedZero) : str="Divided By Zero"; break;
	case(GError::SQRtnegative) : str = "Requested sqrt of a negative number"; break;
	default:
		break;
	}
	return str;
}

#endif // GENGINE_CORE_H 