#include "flib_vec2.h"
#include "transform.h"
#include "uo_box.h"
#include "uo_box_collider.h"
#include "kinematic.h"
#include "trigger.h"
#include "trigger_linked.h"
#include "flib.h"
#include "camera.h"

TTrigerLinked::TTrigerLinked():
	m_pLineSprite(nullptr)
{

}

void TTrigerLinked::DrawBox()
{
	GfxLineSpriteReset(m_pLineSprite);

	GfxLineSpriteSetDrawingColor(m_pLineSprite, EGfxColor_Green);

	const TGfxVec2 tScreenPos = TCamera::GetInstance().GetScreenPos(*this);
		

	TGfxVec2 tAxisX = TGfxVec2(m_tRadius.x, 0);
	TGfxVec2 tAxisY = TGfxVec2(0, m_tRadius.y);

	TGfxVec2 tUR = tScreenPos + tAxisX + tAxisY;
	TGfxVec2 tUL = tScreenPos - tAxisX + tAxisY;
	TGfxVec2 tBL = tScreenPos - tAxisX - tAxisY;
	TGfxVec2 tBR = tScreenPos + tAxisX - tAxisY;

	GfxLineSpriteJumpTo(m_pLineSprite, tUR.x, tUR.y);

	GfxLineSpriteLineTo(m_pLineSprite, tUL.x, tUL.y);
	GfxLineSpriteLineTo(m_pLineSprite, tBL.x, tBL.y);
	GfxLineSpriteLineTo(m_pLineSprite, tBR.x, tBR.y);
	GfxLineSpriteLineTo(m_pLineSprite, tUR.x, tUR.y);
}

void TTrigerLinked::SetRadius(TGfxVec2 tRadius)
{
	m_tRadius = tRadius;
	Synchronise();
}

void TTrigerLinked::Link(const TUoBox * pLinkTarget, EBoxSide eLinkSide, const TGfxVec2 & tTriggerRadius)
{
	//if (m_pLineSprite == nullptr)
	//{
	//	m_pLineSprite = GfxLineSpriteCreate();
	//}

	const TGfxVec2 tLinkTargetPos = pLinkTarget->GetWorldPos();
	const TGfxVec2 tLinkTargetRadius = pLinkTarget->GetRadius();
	const TGfxVec2 tTotalRadius = tLinkTargetRadius + tTriggerRadius;
	const TGfxVec2 tTotalRadiusX = TGfxVec2(tTotalRadius.x, 0);
	const TGfxVec2 tTotalRadiusY = TGfxVec2(0, tTotalRadius.y);
	TGfxVec2 tTriggerWorldPos;
	m_eBoxSide = eLinkSide;
	m_pLinkTarget = pLinkTarget;

	switch (eLinkSide)
	{
		case EBoxSide_Centered:

			tTriggerWorldPos = tLinkTargetPos;

			break;

		case EBoxSide_Left:

			tTriggerWorldPos = tLinkTargetPos - tTotalRadiusX;

			break;

		case EBoxSide_Right:

			tTriggerWorldPos = tLinkTargetPos + tTotalRadiusX;

			break;

		case EBoxSide_Up:

			tTriggerWorldPos = tLinkTargetPos + tTotalRadiusY;

			break;

		case EBoxSide_Down:

			tTriggerWorldPos = tLinkTargetPos - tTotalRadiusY;

			break;
	}

	Initialize(tTriggerWorldPos, tTriggerRadius);
}

void TTrigerLinked::Synchronise()
{
	Link(m_pLinkTarget, m_eBoxSide, m_tRadius);
}

void TTrigerLinked::Update()
{
	Synchronise();
	//DrawBox();
}

void TTrigerLinked::Render()
{
	//GfxSpriteRender(m_pLineSprite);
}

