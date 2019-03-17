#include "GEngineIncludes.h"
#include "GSystem.h"


GSystem::GSystem(void)	:	_pFps(nullptr), _pInput(nullptr),_pGraphics(nullptr), _pSound(nullptr)
{
	_applicationName= "Default Application Name";
}
GSystem::GSystem(LPCSTR applicationName)	:	_pFps(nullptr), _pInput(nullptr),_pGraphics(nullptr), _pSound(nullptr)
{
	_applicationName = applicationName;
}

void GSystem::shutDown()
{
	if(_pSound!= nullptr)
	{
		_pSound->shutDown();
		delete _pSound;
		_pSound = nullptr;
	}

	if(_pGraphics!= nullptr)
	{
		_pGraphics->shutDown();
		delete _pGraphics;
		_pGraphics= nullptr;
	}
	if(_pInput!=nullptr)
	{
		_pInput->shutDown();
		delete _pInput;
		_pInput= nullptr;
	}
	if(_pFps!=nullptr)
	{
		delete _pFps;
		_pFps = nullptr;
	}
	shutDownWindows();
	return;
}

//--------------------------------------------------------------------------------------
// Init Window And Devices
//--------------------------------------------------------------------------------------
HRESULT GSystem::initialize()
{	
	HRESULT result;
	int screenWidth=0, screenHeight=0;
	
	// Initialize the windows api.
	initWindow(screenWidth, screenHeight);

	//query CPU frequency, init high resolution timers and random seed
	GHighResTimer::init();	
	DBG_OUTPT("CPU frequency = " << GHighResTimer::cyclesPerSecond()/1000.0f <<" MHz" );
	GMath::GRandom::seed( static_cast<GMath::uint32>(GMath::GTime::currentTime().msecs()) );

	//Create Fps counter object
	_pFps = new GFps();
	if(_pFps==nullptr)					{return E_FAIL;}

	// Initialize the input system
	_pInput = new GInput();
	if(_pInput==nullptr)				{return E_FAIL;}

	result = _pInput->initialize(_hInstance,_hWnd,screenWidth,screenHeight);
	if( FAILED( result ) )
	{
		MessageBox(_hWnd, "Could not initialize the input object.", "Error", MB_OK);
		DBG_OUTPT("Failed to initialize Input Device");
		return result;
	}

	// Initialize  the graphics system.  This object will handle rendering all the graphics for this application.
	_pGraphics = new GGraphics();
	if(_pGraphics==nullptr)				{return E_FAIL;}

	bool res = _pGraphics->initialize(screenWidth, screenHeight, _hWnd);
	if( !res )
	{
		DBG_OUTPT("Failed to initialize Graphic Device");
		return E_FAIL;
	}
	
	//Initialize the Sound System
	_pSound = new GSound;
	if(_pSound==nullptr)				{return E_FAIL;}

	res = _pSound->initialize(_hWnd);
	if( !res )
	{
		MessageBox(_hWnd, "Could not initialize Direct Sound.", "Error", MB_OK);
		DBG_OUTPT("Failed to initialize Sound System");
		return E_FAIL;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Application Loop
//--------------------------------------------------------------------------------------
void GSystem::run()
{
	MSG msg;
	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	//variables I need
	bool quit=false;
	GHighResTimer globalTimer;
	float dt = 0.01666666666f;					//==1/60;	a common value for screen frequency
	GMath::GTime deltaT(dt);
	int mouseX, mouseY;
	_pFps->reset();
	
	// Loop until there is a quit message from the window or the user.
	while(msg.message != WM_QUIT && quit == false)
	{
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Game Loop
		else
		{
			quit = !_pInput->update();
			_pInput->getMouseLocation(mouseX,mouseY);

			if(_pInput->IsKeyDown(GKeys::Escape))
			{
				quit =true;
				DBG_OUTPT("hai premuto ESC, esco dal programma");
				break;
			}
			if(_pInput->IsKeyDown(GKeys::P))
			{
				DBG_OUTPT("hai premuto P, metto in pausa il programma");
				if(globalTimer.isPaused() )
					globalTimer.pause(false);
				else
					globalTimer.pause(true);
			}

			if(_pInput->IsKeyDown(GKeys::S))
			{
				_pSound->playWaveFile();
			}

			globalTimer.startCounting();

			_pGraphics->setFps(_pFps->getFps() );
			_pGraphics->updateState(dt,mouseX,mouseY);
			quit= quit || !_pGraphics->render();

			deltaT = globalTimer.update();
			_pFps->update(deltaT );
			dt = static_cast<float>(deltaT.inSeconds() );
		}
	}
	return;
}

LRESULT CALLBACK GSystem::messageHandler( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	return DefWindowProc(hWnd,message,wParam,lParam);
}

//--------------------------------------------------------------------------------------
// Initialize Window
//--------------------------------------------------------------------------------------
HRESULT GSystem::initWindow(int&screenWidth, int&screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.
	_gpApplicationHandle = this;

	// Get the instance of this application.
	_hInstance = GetModuleHandle(NULL);

	// Give the application a name.
	if(_applicationName== "")
	{
		_applicationName = "GEngine";
	}

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);
	
	// Register the window class.
	if(!RegisterClassEx(&wc))
		return E_ABORT;

	// Determine the resolution of the clients desktop screen.
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(g_IsFullScreen)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 1200x600 resolution.
		screenWidth  = 1200;
		screenHeight = 600;
		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	
	_hWnd = CreateWindowExA(WS_EX_APPWINDOW, _applicationName, _applicationName, 
				WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
				posX, posY, screenWidth, screenHeight, NULL, NULL, _hInstance, NULL);

	if(!_hWnd )
		return E_HANDLE;

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(_hWnd, SW_SHOW);
	SetForegroundWindow(_hWnd);
	SetFocus(_hWnd);

	// Hide or Show the mouse cursor.
	ShowCursor(!g_IsMouseHide);

	return S_OK;
}

void GSystem::shutDownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(g_IsFullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(_hWnd);
	_hWnd= NULL;

	// Remove the application instance.
	UnregisterClass(_applicationName, _hInstance);
	_hInstance = NULL;

	// Release the pointer to this class.
	_gpApplicationHandle = nullptr;

	return;
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}
		// All other messages pass to the message handler in the system class.
		default:
		{
			return _gpApplicationHandle->messageHandler(hwnd, message, wParam, lParam);
		}
	}
}