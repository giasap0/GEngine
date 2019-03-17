#pragma once
#ifndef GFONT_H
#define GFONT_H

#include <DirectXMath.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <GWString.h>
#include <GMath.h>
#include "Shaders\GColors.h"

using DirectX::SpriteFont;
using DirectX::SpriteBatch;
using DirectX::SpriteEffects;
using GMath::GVector2;
using GMath::Radians;

//allow to render text (use with DirectX::SpriteBatch)
class GFont
{
public:
	GFont();
	virtual ~GFont(void);

	bool initialize(_In_ ID3D11Device* device, _In_z_ wchar_t const* fileName);
	void shutDown();

	void DrawString(_In_ SpriteBatch* spriteBatch, GWString text, GVector2 const& position, float scale = 1, GColor color = GColors::White, Radians rotation = Radians(), GVector2 const& origin = GVector2(0,0), SpriteEffects effects =DirectX:: SpriteEffects_None, float layerDepth = 0);
	void DrawString(_In_ SpriteBatch* spriteBatch, GWString text, GVector2 const& position, GVector2 const& scale, GColor color, Radians rotation, GVector2 const& origin, SpriteEffects effects = DirectX::SpriteEffects_None, float layerDepth = 0);

	void DrawString(_In_ SpriteBatch* spriteBatch, _In_z_ wchar_t const* text, DirectX::XMFLOAT2 const& position,  float scale = 1, DirectX::FXMVECTOR color = DirectX::Colors::White, float rotation = 0, DirectX::XMFLOAT2 const& origin = DirectX::XMFLOAT2(0,0),SpriteEffects effects =DirectX:: SpriteEffects_None, float layerDepth = 0);
    void DrawString(_In_ SpriteBatch* spriteBatch, _In_z_ wchar_t const* text, DirectX::XMFLOAT2 const& position,  DirectX::XMFLOAT2 const& scale, DirectX::FXMVECTOR color, float rotation, DirectX::XMFLOAT2 const& origin, SpriteEffects effects = DirectX::SpriteEffects_None, float layerDepth = 0);

    
	inline DirectX::XMVECTOR MeasureString(_In_z_ wchar_t const* text) const					{return _pFont->MeasureString(text);}
	inline DirectX::XMVECTOR MeasureString(GWString text) const									{return _pFont->MeasureString(text.data() );}

	inline float GetLineSpacing() const													{return _pFont->GetLineSpacing();}
	inline void SetLineSpacing(float spacing)											{_pFont->SetLineSpacing(spacing);}

	inline wchar_t GetDefaultCharacter() const											{return _pFont->GetDefaultCharacter();}
	inline void SetDefaultCharacter(wchar_t character)									{_pFont->SetDefaultCharacter(character);}

	inline bool ContainsCharacter(wchar_t character) const								{return _pFont->ContainsCharacter(character);}

private:
	SpriteFont*		_pFont;

	 // Prevent copying.
     GFont(GFont const&);
     GFont& operator= (GFont const&);
};

#endif	//GFONT_H

/*
        SpriteFont(SpriteFont&& moveFrom);
        SpriteFont& operator= (SpriteFont&& moveFrom);
*/