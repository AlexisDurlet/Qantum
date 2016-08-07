#include "flib_vec2.h"
#include "transform.h"
#include "sprite.h"
#include "flib.h"
#include "camera.h"
#include "math.h"
#include "e_orientation.h"

TSprite::TSprite() :
	m_pSprite(nullptr),
	m_pTransform(nullptr),
	m_tSize(TGfxVec2(0, 0))
{

}

TSprite::~TSprite() 
{
	if (m_pSprite != nullptr)
	{
		GfxSpriteDestroy(m_pSprite);
		m_pSprite = nullptr;
	}
}

void TSprite::SetScale(const TGfxVec2 & tScale)
{
	GfxSpriteSetScale(m_pSprite, tScale.x, tScale.y);
	m_tScale = TGfxVec2(fabsf(tScale.x), fabsf(tScale.y));
}

void TSprite::Initialize(TTransform * pTransform, const TGfxTexture * pTexture, const TGfxVec2 & tCutOut, const TGfxVec2 & tSize, const TGfxVec2 & tScale)
{
	m_pTransform = pTransform;

	m_pSprite = GfxSpriteCreate(pTexture);

	GfxSpriteSetCutout(m_pSprite, static_cast<int>(tCutOut.x), static_cast<int>(tCutOut.y), static_cast<int>(tSize.x), static_cast<int>(tSize.y));
	m_tInitialCutOut = tCutOut;
	m_tSize = tSize;

	SetScale(tScale);

	GfxSpriteSetPivot(m_pSprite, GfxSpriteGetSizeX(m_pSprite) / 2.0f, GfxSpriteGetSizeY(m_pSprite) / 2.0f);
}

void TSprite::SetCutOut(const TGfxVec2 & tCutOut)
{
	GfxSpriteSetCutout(m_pSprite, static_cast<int>(tCutOut.x), static_cast<int>(tCutOut.y), static_cast<int>(m_tSize.x), static_cast<int>(m_tSize.y));
}


void TSprite::SetFilter(bool bFilter)
{
	GfxSpriteSetFilteringEnabled(m_pSprite, bFilter);
}

void TSprite::SetColor(unsigned int iColor)
{
	GfxSpriteSetColor(m_pSprite, iColor);
}

void TSprite::ResetCutout()
{
	GfxSpriteSetCutout(m_pSprite, static_cast<int>(m_tInitialCutOut.x), static_cast<int>(m_tInitialCutOut.y), static_cast<int>(m_tSize.x), static_cast<int>(m_tSize.y));
}

void TSprite::Flip(const EOrientation eOrientation)
{
	switch (eOrientation)
	{
		case EOrientation_Left:

			SetScale(TGfxVec2(-m_tScale.x, m_tScale.y));

			break;

		case EOrientation_Right:

			SetScale(TGfxVec2(m_tScale.x, m_tScale.y));

			break;

		case EOrientation_Up:

			SetScale(TGfxVec2(m_tScale.x, -m_tScale.y));

			break;

		case EOrientation_Down:

			SetScale(TGfxVec2(m_tScale.x, m_tScale.y));

			break;
	}
}

void TSprite::Rotate(const EOrientation eOrientation)
{
	switch (eOrientation)
	{
		case EOrientation_Left:

			GfxSpriteSetAngle(m_pSprite, GfxMathDegToRad(-90));

			break;

		case EOrientation_Right:

			GfxSpriteSetAngle(m_pSprite, GfxMathDegToRad(90));

			break;

		case EOrientation_Up:

			Flip(eOrientation);

			break;

		case EOrientation_Down:

			Flip(eOrientation);

			break;
	}
}


void TSprite::Update()
{
	const TGfxVec2 tScreenPos = TCamera::GetInstance().GetScreenPos(*m_pTransform);

	GfxSpriteSetPosition(m_pSprite, tScreenPos.x, tScreenPos.y);
}

void TSprite::Render()
{
	if (m_pSprite != nullptr)
	{
		GfxSpriteRender(m_pSprite);
	}
}

void TSprite::Destroy()
{
	GfxSpriteDestroy(m_pSprite);
}
