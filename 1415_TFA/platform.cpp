#include "flib_vec2.h"
#include "transform.h"
#include "uo_box.h"
#include "sprite.h"
#include "platform.h"
#include "time.h"
#include "constants.h"
#include "math.h"
#include "e_orientation.h"

void TPlatform::Initialize(const TGfxTexture * pTexture, const TGfxVec2 & tWorldPos, const TGfxVec2 & tRadius)
{
	InitializeUoBox(tWorldPos, tRadius);

	const float fCutoutSizeX = fmaxf(m_tRadius.x * 2 * (1/SCALE), m_tRadius.y * 2 * (1/SCALE));

	m_tSprite.Initialize(this, pTexture, PLATFORM_CUTOUT, TGfxVec2(fCutoutSizeX, float(TILE_SIZE)), TGfxVec2(SCALE, SCALE));
	m_tSprite.SetFilter(false);

	if (fCutoutSizeX == m_tRadius.y * 2 * (1 / SCALE))
	{
		m_tSprite.Rotate(EOrientation_Left);
	}
}

void TPlatform::Update(const TTime & tTime)
{
	(void)tTime;
	m_tSprite.Update();
}

void TPlatform::Render()
{
	m_tSprite.Render();
}
