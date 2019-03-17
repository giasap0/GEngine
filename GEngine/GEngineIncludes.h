#pragma once
#ifndef GENGINEINCLUDES_H
#define GENGINEINCLUDES_H

#define WIN32_LEAN_AND_MEAN             // Escludere gli elementi utilizzati di rado dalle intestazioni di Windows
// File di intestazione di Windows:
#include <windows.h>

// File di intestazione Runtime C
#include <stdlib.h>
#include <malloc.h>
#include <tchar.h>

//intestazioni aggiuntive richieste dal programma
#include <memory>
#include <d3d11.h>
#include <GEngineCore.h>
#include <GHighResTimer.h>
#include "GLogger.h"


#endif	//GENGINEINCLUDES_H