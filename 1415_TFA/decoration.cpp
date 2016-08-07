#include "flib_vec2.h"
#include "transform.h"
#include "sprite.h"
#include "decoration.h"
#include "time.h"

void TDecoration::InitializeSprite(const TGfxTexture * pTexture, const TGfxVec2 & tCutOut, const TGfxVec2 & tSize, const TGfxVec2 & tScale)
{
	m_tSprite.Initialize(this, pTexture, tCutOut, tSize, tScale);
}

void TDecoration::Update()
{
	m_tSprite.Update();
}

void TDecoration::Render()
{
	m_tSprite.Render();
}
