#include "flib_vec2.h"
#include "camera.h"
#include "transform.h"
#include "flib.h"

TCamera::TCamera() :
	m_pFocus(nullptr)
{

}

TGfxVec2 TCamera::ConvertWorldPosToScreenPos(const TGfxVec2 & tWorldPos) const
{
	const TGfxVec2 tRelativeToFocusPos = tWorldPos - m_pFocus->GetWorldPos();
	const TGfxVec2 tScreenPos = TGfxVec2(tRelativeToFocusPos.x + GfxGetDisplaySizeX() / 2.0f, -tRelativeToFocusPos.y + GfxGetDisplaySizeY() / 2.0f);

	return tScreenPos;
}

TGfxVec2 TCamera::ConvertScreenPosToWorldPos(const TGfxVec2 & tScreenPos) const
{
	const TGfxVec2 tRelativeToFocusPos = TGfxVec2(tScreenPos.x - GfxGetDisplaySizeX() / 2.0f, -tScreenPos.y + GfxGetDisplaySizeY() / 2.0f);
	const TGfxVec2 tWorldPos = tRelativeToFocusPos + m_pFocus->GetWorldPos();

	return tWorldPos;
}

TGfxVec2 TCamera::GetScreenPos(const TTransform & tTransform) const 
{
	const TGfxVec2 tScreenPos = ConvertWorldPosToScreenPos(tTransform.GetWorldPos());

	return tScreenPos;
}
