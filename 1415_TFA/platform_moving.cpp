#include "flib_vec2.h"
#include "transform.h"
#include "uo_box.h"
#include "uo_box_collider.h"
#include "sprite.h"
#include "platform.h"
#include "kinematic.h"
#include "time.h"
#include "kinematic_cycle.h"
#include "trigger.h"
#include "trigger_linked.h"
#include "platform_moving.h"
#include "blink_vector.h"
#include "probe.h"
#include "kinematic_gravity.h"
#include "uo_box_physical.h"
#include "animation.h"
#include "decoration.h"
#include "decoration_animated.h"
#include "player.h"
#include "lerp.h"
#include "constants.h"
#include "e_player_tp_state.h"
#include "math.h"


void TPlatformMoving::CheckPhysBoxOnGround(TUoBoxPhysical & tUoBoxPhysical)
{
	if (m_tGround.CheckTrigger(tUoBoxPhysical))
	{
		tUoBoxPhysical.m_tExternalSpeed = m_tKinematic.m_tSpeed;
		tUoBoxPhysical.m_pExternalInterface = &m_tGround;
	}
	else if (tUoBoxPhysical.m_pExternalInterface == &m_tGround)
	{
		tUoBoxPhysical.m_tExternalSpeed = TGfxVec2(0, 0);
	}
}

void TPlatformMoving::Initialize(const TGfxTexture * pTexture, const TGfxVec2 & tWorldPos, const TGfxVec2 & tRadius)
{
	TPlatform::Initialize(pTexture, tWorldPos, tRadius);

	m_tGround.Link(this, EBoxSide_Up, TGfxVec2(m_tRadius.x, GROUND_INTERFACE_THICKNESS));
}

void TPlatformMoving::InitializeMovement(const TGfxVec2 & tDestination, const int iStopTime, const float fSpeed)
{
	m_tKinematic.Initialize(this, tDestination, iStopTime, fSpeed);
}

void TPlatformMoving::Update(const TTime & tTime)
{
	m_tKinematic.Update(tTime);
	TPlatform::Update(tTime);

	m_tGround.Update();

	CheckPhysBoxOnGround(TPlayer::GetInstance().GetCastToUoBoxPhys());
}

void TPlatformMoving::Render()
{
	TPlatform::Render();

	m_tGround.Render();
}
