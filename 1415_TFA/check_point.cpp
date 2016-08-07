#include "flib_vec2.h"
#include "transform.h"
#include "sprite.h"
#include "uo_box.h"
#include "uo_box_collider.h"
#include "kinematic.h"
#include "trigger.h"
#include "time.h"
#include "animation.h"
#include "check_point.h"
#include "constants.h"
#include "blink_vector.h"
#include "kinematic.h"
#include "kinematic_gravity.h"
#include "trigger_linked.h"
#include "uo_box_physical.h"
#include "sprite.h"
#include "decoration.h"
#include "decoration_animated.h"
#include "player.h"

void TCheckPoint::Initialize(TGfxTexture * pTexture, const TGfxVec2 & tWorldPos)
{
	TTrigger::Initialize(tWorldPos, CHECKPOINT_RADIUS);

	m_tSprite.Initialize(this, pTexture, CHECKPOINT_INITIAL_CUTOUT, CHECKPOINT_CUTOUT_SIZE, TGfxVec2(SCALE, SCALE));

	m_tAnimation.Initialize(&m_tSprite, CHECKPOINT_RESPAWN_CUTOUT, ECutOutIncrement_X_Positive, CHECKPOINT_RESPAWN_TIME_PER_FRAME, CHECKPOINT_RESPAWN_FRAME_NUMBER);

	m_bActive = false;
	m_bRespawning = false;
	m_bEndCheckPoint = false;
}

void TCheckPoint::Update()
{
	m_tSprite.Update();
	
	if (CheckTrigger(*TPlayer::GetInstance().GetCastToUoBox()) && !m_bActive)
	{
		m_bActive = true;
		m_tSprite.SetCutOut(CHECKPOINT_ACTIVATED_CUTOUT);
		TPlayer::GetInstance().SetCheckPoint(this);

		if (m_bEndCheckPoint)
		{
			TPlayer::GetInstance().ExitLevel();
		}
	}

	if (m_bRespawning)
	{
		m_tAnimation.Update();
	}
}

void TCheckPoint::Render()
{
	m_tSprite.Render();
}

void TCheckPoint::StartRespawn()
{
	m_bRespawning = true;
	m_tAnimation.Start();
}

void TCheckPoint::StopRespawn()
{
	m_bRespawning = false;
	m_tSprite.SetCutOut(CHECKPOINT_ACTIVATED_CUTOUT);
}
