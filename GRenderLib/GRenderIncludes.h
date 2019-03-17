#pragma once
#ifndef GRENDERINCLUDES_H
#define GRENDERINCLUDES_H

#define WIN32_LEAN_AND_MEAN             // Escludere gli elementi utilizzati di rado dalle intestazioni di Windows
// File di intestazione di Windows:
#include <windows.h>
#include <stdlib.h>

//riferimenti alle DX (win8 sdk)
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;
using DirectX::XMFLOAT4;
using DirectX::XMVECTOR;
using DirectX::XMMATRIX;

//intestazioni aggiuntive richieste dal programma
#include <GEngineCore.h>
#include <GMath.h>


#endif	//GRENDERINCLUDES_H