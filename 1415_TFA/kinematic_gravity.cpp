#include "flib_vec2.h"
#include "kinematic.h"
#include "kinematic_gravity.h"
#include "constants.h"

TKinematicGravity::TKinematicGravity():
	m_bGravityOn(false),
	m_bOnGround (false)
{

}

void TKinematicGravity::MoveY(const TTime &tTime)
{
	if (!m_bOnGround && !m_bGravityOn)
	{
		m_tAcceleration += TGfxVec2(0, -GRAVITY);
		m_bGravityOn = true;
	}
	
	if (m_bOnGround  && m_bGravityOn)
	{
		m_tAcceleration -= TGfxVec2(0, -GRAVITY);
		m_bGravityOn = false;
	}

	TKinematic::MoveY(tTime);
}

