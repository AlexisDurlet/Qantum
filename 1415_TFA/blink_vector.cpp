#include "flib_vec2.h"
#include "transform.h"
#include "blink_vector.h"
#include "uo_box.h"
#include "uo_box_collider.h"
#include "kinematic.h"
#include "kinematic_gravity.h"
#include "trigger.h"
#include "trigger_linked.h"
#include "uo_box_physical.h"
#include "time.h"
#include "animation.h"
#include "sprite.h"
#include "decoration.h"
#include "decoration_animated.h"
#include "player.h"
#include "flib.h"
#include "probe.h"
#include "constants.h"
#include "math.h"
#include "camera.h"
#include "lerp.h"

void TBlinkVector::DrawVector(const TGfxVec2 tOrigin, const TGfxVec2 tDestination)
{
	const TGfxVec2 tVector = tDestination - tOrigin;

	if (tVector != TGfxVec2(0, 0))
	{
		GfxLineSpriteSetDrawingColor(m_pLineSprite, m_iDrawingColor);

		GfxLineSpriteJumpTo(m_pLineSprite, tOrigin.x, tOrigin.y);
		GfxLineSpriteLineTo(m_pLineSprite, tDestination.x, tDestination.y);

		TGfxVec2 ArrowHead = tVector.Normalize() * 20;

		ArrowHead = ArrowHead.Rotate(160);
		GfxLineSpriteLineTo(m_pLineSprite, tDestination.x + ArrowHead.x, tDestination.y + ArrowHead.y);

		GfxLineSpriteJumpTo(m_pLineSprite, tDestination.x, tDestination.y);

		ArrowHead = ArrowHead.Rotate(-2 * 160);
		GfxLineSpriteLineTo(m_pLineSprite, tDestination.x + ArrowHead.x, tDestination.y + ArrowHead.y);
	}
}

void TBlinkVector::Initialize()
{
	m_tWorldPos = TPlayer::GetInstance().GetWorldPos();
	m_tTarget = m_tWorldPos;

	m_fTPtime = 0;

	m_fStepLenght = BLINK_STEP_LENGHT;

	m_bActive = false;

	m_pLineSprite = GfxLineSpriteCreate();
}

void TBlinkVector::Start(const TGfxVec2 & tTarget, const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber)
{
	m_bActive = true;
	m_bTargetChanged = false;

	m_tWorldPos = TPlayer::GetInstance().GetWorldPos();
	m_tTarget = tTarget;
	m_tVector = tTarget - m_tWorldPos;

	int iStepNumber = int(m_tVector.Length() / m_fStepLenght);

	TProbe tProbe;
	tProbe.Initialize(m_tWorldPos);
	const TUoBox * tProbeResult = tProbe.RayProbe(tBoxesCollided, iBoxesCollidedNumber, m_tTarget, iStepNumber);
	if (tProbeResult == nullptr)
	{
		m_bValidTarget = true;
		m_iDrawingColor = EGfxColor_Green;

		m_tTPSpeed = Lerpv(m_tWorldPos, m_tTarget, BLINK_SPEED);
		m_fTPtime = Lerpf(m_tWorldPos, m_tTarget, BLINK_SPEED) * 1000;
	}
	else
	{
		m_bValidTarget = false;
		m_iDrawingColor = EGfxColor_Red;
	}
}

void TBlinkVector::Update(const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber)
{

	m_tWorldPos = TPlayer::GetInstance().GetWorldPos();

	if (m_bActive)
	{
		GfxLineSpriteReset(m_pLineSprite);

		TGfxVec2 m_tScreenPos = TCamera::GetInstance().GetScreenPos(*this);

		TTransform tTargetTransform;
		tTargetTransform.InitializeTransform(m_tTarget);
		TGfxVec2 m_tTargetScreenPos = TCamera::GetInstance().GetScreenPos(tTargetTransform);

		DrawVector(m_tScreenPos, m_tTargetScreenPos);

		if (m_bValidTarget)
		{
			m_tVector = m_tTarget - m_tWorldPos;

			int iStepNumber = int(m_tVector.Length() / m_fStepLenght);

			TProbe tProbe;
			tProbe.Initialize(m_tWorldPos);
			const TUoBox * tProbeResult = tProbe.RayProbe(tBoxesCollided, iBoxesCollidedNumber, m_tTarget, iStepNumber);

			if (tProbeResult != nullptr)
			{
				m_tTarget = tProbe.GetLastValidPos();
				m_fTPtime = Lerpf(m_tWorldPos, m_tTarget, BLINK_SPEED) * 1000;
				m_bTargetChanged = true;
			}

			if (m_tVector.x * m_tTPSpeed.x < 0 || m_tVector.y * m_tTPSpeed.y < 0) //prevent to continue to blink past the target no matter what
			{
				m_fTPtime = 0;
			}
			
		}
	}
}

void TBlinkVector::Stop()
{
	GfxLineSpriteReset(m_pLineSprite);

	m_tWorldPos = TPlayer::GetInstance().GetWorldPos();
	m_tTarget = m_tWorldPos;

	m_bActive = false;
	m_bValidTarget = false;
}

void TBlinkVector::Render()
{
	if (m_bActive)
	{
		GfxSpriteRender(m_pLineSprite);
	}
}
