#include "GEngineIncludes.h"
#include "GMainWindow.h"
#include "GSystem.h"

//--------------------------------------------------------------------------------------
// Linking
//--------------------------------------------------------------------------------------
#pragma comment(lib, "d3d11.lib")

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into loop
//--------------------------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int cmdShow)
{
	UNREFERENCED_PARAMETER( prevInstance );
	UNREFERENCED_PARAMETER( cmdLine );

	//set up checks for memory leaks before app exits
	int tempDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tempDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tempDbgFlag);

	// Create the system object.
	GSystem* System = new GSystem("appName");
	if(!System)
	{
		return -1;
	}

	// Initialize and run the system object.
	HRESULT result = System->initialize();
	if( FAILED( result ) )
	{
		DBG_OUTPT("Failed to initialize System");
        return -1;
	}

	//Application and game loop
	System->run();

	// Shutdown and release the system object.
	System->shutDown();
	delete System;
	System = nullptr;

	return 0;
}
