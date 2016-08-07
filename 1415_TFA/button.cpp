#include "flib_vec2.h"
#include "transform.h"
#include "uo_box.h"
#include "sprite.h"
#include "button.h"
#include "probe.h"
#include "flib.h"
#include "camera.h"
#include "constants.h"


void TButton::InitializeButton(const TGfxVec2 & tWorldPos, const TGfxVec2 & tRadius)
{
	InitializeUoBox(tWorldPos, tRadius);
	m_bHasBeenPressed = false;
}

void TButton::InitializeSprite(TGfxTexture * pTexture, const TGfxVec2 & tCutout, const TGfxVec2 & tPressedCutout)
{
	m_tSprite.Initialize(this, pTexture, tCutout, TGfxVec2(m_tRadius.x * (1 / SCALE) * 2, m_tRadius.y * (1 / SCALE) * 2), TGfxVec2(SCALE, SCALE));
	m_tPressedCutout = tPressedCutout;
}

bool TButton::Update()
{
	m_tSprite.Update();

	TGfxVec2 tMousePos = TGfxVec2(float(GfxGetCurrentMouseX()), float(GfxGetCurrentMouseY()));
	tMousePos = TCamera::GetInstance().ConvertScreenPosToWorldPos(tMousePos);

	bool bIsjustPressed = GfxInputIsJustPressed(EGfxInputID_MouseLeft);
	bool bIsjustReleased = GfxInputIsJustReleased(EGfxInputID_MouseLeft);

	if (bIsjustPressed)
	{
		TProbe tProbe;
		tProbe.Initialize(tMousePos);
		if (tProbe.Probe(this, 1) != nullptr)
		{
			m_bHasBeenPressed = true;
			m_tSprite.SetCutOut(m_tPressedCutout);
		}
	}
	else if (bIsjustReleased && m_bHasBeenPressed)
	{
		m_tSprite.ResetCutout();
		m_bHasBeenPressed = false;

		TProbe tProbe;
		tProbe.Initialize(tMousePos);
		if (tProbe.Probe(this, 1) != nullptr)
		{
			return true;
		}
	}



	return false;
}

void TButton::Render()
{
	m_tSprite.Render();
}

