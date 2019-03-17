#pragma once
#ifndef GRENDERINCLUDES_H
#define GRENDERINCLUDES_H

#define WIN32_LEAN_AND_MEAN             // Escludere gli elementi utilizzati di rado dalle intestazioni di Windows
//Windows:
#include <windows.h>
#include <stdlib.h>

//DirectX (win8 sdk)
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

//custom common libs
#include <GEngineCore.h>
#include <GMath.h>

//common types
using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;
using DirectX::XMFLOAT4;
using DirectX::XMVECTOR;
using DirectX::XMMATRIX;
using GMath::Int32;
using GMath::uint32;
using GMath::Real;
using GMath::GVector3;
using GMath::GVector4;
using GMath::GMatrix4x4;


#endif	//GRENDERINCLUDES_H