#include "flib_vec2.h"
#include "transform.h"
#include "uo_box.h"
#include "uo_box_collider.h"
#include "kinematic.h"
#include "kinematic_gravity.h"
#include "trigger.h"
#include "trigger_linked.h"
#include "uo_box_physical.h"
#include "time.h"
#include"probe.h"
#include "constants.h"
#include "flib.h"
#include "math.h"

bool TUoBoxPhysical::CheckForGround(const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber)
{
	bool bGroundDetected = false;

	for (int i = 0; i < iBoxesCollidedNumber; i++)
	{
		if (m_tInterfaceWithGround.CheckTrigger(tBoxesCollided[i]))
		{
			bGroundDetected = true;
		}
	}

	return bGroundDetected;
}

void TUoBoxPhysical::HandleGroundException()
{
	bool bStuckFallingOnCorner = m_tKinematicGravity.GetGravityOn() && m_tKinematicGravity.m_tSpeed.y == 0;

	if (bStuckFallingOnCorner)
	{
		m_iGroundExceptionFrameNumber++;
	}
	else if (!bStuckFallingOnCorner && m_bGroundException)
	{
		m_iGroundExceptionFrameNumber = 0;
		m_bGroundException = false;
		m_tRadius.x += GROUND_INTERFACE_LENGHT_TOLERANCE;
	}

	if (m_iGroundExceptionFrameNumber > 1 && !m_bGroundException)
	{
		m_bGroundException = true;
		m_tRadius.x -= GROUND_INTERFACE_LENGHT_TOLERANCE;
	}
}

void TUoBoxPhysical::InitializeUoBoxPhysical(const TGfxVec2 & tWorldPos, const TGfxVec2 & tRadius, const TGfxVec2 & tSpeed, const TGfxVec2 & tAcceleration)
{
	InitializeUoBox(tWorldPos, tRadius);

	m_tKinematicGravity.m_tSpeed = tSpeed;
	m_tKinematicGravity.m_tAcceleration = tAcceleration;
	m_tExternalSpeed = TGfxVec2(0, 0);

	m_tInterfaceWithGround.Link(this, EBoxSide_Down, TGfxVec2(m_tRadius.x - GROUND_INTERFACE_LENGHT_TOLERANCE, GROUND_INTERFACE_THICKNESS));

	m_bCollisionX = false;
	m_bGroundException = false;
	m_iGroundExceptionFrameNumber = 0;

	m_pExternalInterface = nullptr;
}

void TUoBoxPhysical::UpdatePhysical(const TTime & tTime, const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber)
{
	m_tKinematicGravity.m_bOnGround = CheckForGround(tBoxesCollided, iBoxesCollidedNumber);

	HandleGroundException();

	// X Movement
	m_bCollisionX = false;
	m_tKinematicGravity.MoveX(tTime);
	
	for (int i = 0; i < iBoxesCollidedNumber; i++)
	{
		if (Collision(tBoxesCollided[i]))
		{
			m_tKinematicGravity.m_tSpeed.x -= m_tExternalSpeed.x;
			m_tExternalSpeed = TGfxVec2(0, m_tExternalSpeed.y);
			m_tKinematicGravity.m_tSpeed.x = 0;
			m_tKinematicGravity.m_tAcceleration.x = 0;
			m_bCollisionX = true;
		}

		CollisionResolveX();
	}


	// Y Movement
	m_tKinematicGravity.MoveY(tTime);

	for (int i = 0; i < iBoxesCollidedNumber; i++)
	{
		if (Collision(tBoxesCollided[i]))
		{
			m_tKinematicGravity.m_tSpeed.y -= m_tExternalSpeed.y;
			m_tExternalSpeed = TGfxVec2(m_tExternalSpeed.x, 0);
			m_tKinematicGravity.m_tSpeed.y = 0;

			if (m_tKinematicGravity.GetGravityOn())
			{
				m_tKinematicGravity.m_tAcceleration.y = -GRAVITY; //remove all y acceleration except gravity
			}
			else
			{
				m_tKinematicGravity.m_tAcceleration.y = 0;
			}
		}

		CollisionResolveY();
	}

	m_tInterfaceWithGround.Update();
}

void TUoBoxPhysical::UpdateForced(const TTime & tTime, const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber)
{
	for (int i = 0; i < iBoxesCollidedNumber; i++)
	{
		ForceCollision(tBoxesCollided[i]);
	}

	m_tInterfaceWithGround.Update();

	m_tKinematicGravity.MoveX(tTime);
	m_tKinematicGravity.MoveY(tTime);

	m_bCollisionX = false;

}

void TUoBoxPhysical::Update(const TTime & tTime, const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber)
{
	if (m_bPhysical)
	{
		m_tKinematicGravity.m_tSpeed += m_tExternalSpeed;
	}

	if (m_bPhysical || m_tKinematicGravity.m_tSpeed != TGfxVec2(0, 0))
	{
		UpdatePhysical(tTime, tBoxesCollided, iBoxesCollidedNumber);
	}
	else
	{
		UpdateForced(tTime, tBoxesCollided, iBoxesCollidedNumber);
	}

	if (m_bPhysical)
	{
		m_tKinematicGravity.m_tSpeed -= m_tExternalSpeed;
	}
}

void TUoBoxPhysical::ResynchGroundInterface()
{
	m_tInterfaceWithGround.Update();
}
