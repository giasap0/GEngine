#include "GFont.h"

using DirectX::XMFLOAT2;
using DirectX::FXMVECTOR;
using DirectX::GXMVECTOR;
using DirectX::XMVECTOR;


GFont::GFont(): _pFont(nullptr)
{
}


GFont::~GFont(void)
{
}

bool GFont::initialize(_In_ ID3D11Device* device, _In_z_ wchar_t const* fileName)
{
	_pFont = new DirectX::SpriteFont(device,fileName);
	if(_pFont == nullptr)
	{
		return false;
	}
	return true;
}

void GFont::shutDown()
{
	if(_pFont!=nullptr)
	{
		delete _pFont;
		_pFont = nullptr;
	}
}


void GFont::DrawString(_In_ SpriteBatch* spriteBatch, GWString text, GVector2 const& position, float scale, GColor color, Radians rotation, GVector2 const& origin, SpriteEffects effects, float layerDepth)
{
	XMFLOAT2 pos, orig;
	pos.x = position.x();
	pos.y = position.y();
	orig.x = origin.x();
	orig.y = origin.y();
	FXMVECTOR c = {color.red , color.green , color.blue , color.alpha};
	_pFont->DrawString(spriteBatch,text.data(),pos,c,rotation.toReal(),orig,scale,effects,layerDepth);
}
void GFont::DrawString(_In_ SpriteBatch* spriteBatch, GWString text, GVector2 const& position,GVector2 const& scale, GColor color, Radians rotation, GVector2 const& origin, SpriteEffects effects, float layerDepth)
{
	XMFLOAT2 pos, orig, sc;
	pos.x = position.x();
	pos.y = position.y();
	orig.x = origin.x();
	orig.y = origin.y();
	sc.x = scale.x();
	sc.y = scale.y();
	FXMVECTOR c = {color.red , color.green , color.blue , color.alpha};
	_pFont->DrawString(spriteBatch,text.data(),pos,c,rotation.toReal(),orig,sc,effects,layerDepth);
}

void GFont::DrawString(_In_ SpriteBatch* spriteBatch, _In_z_ wchar_t const*text, XMFLOAT2 const& position, float scale, FXMVECTOR color, float rotation, XMFLOAT2 const& origin, SpriteEffects effects, float layerDepth)
{
	_pFont->DrawString(spriteBatch,text,position,color,rotation,origin,scale,effects,layerDepth);
}
void GFont::DrawString(_In_ SpriteBatch* spriteBatch, _In_z_ wchar_t const* text, XMFLOAT2 const& position, XMFLOAT2 const& scale, FXMVECTOR color, float rotation, XMFLOAT2 const& origin, SpriteEffects effects, float layerDepth)
{
	_pFont->DrawString(spriteBatch,text,position,color,rotation,origin,scale,effects,layerDepth);
}
