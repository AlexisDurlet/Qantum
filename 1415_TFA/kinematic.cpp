#include "flib_vec2.h"
#include "kinematic.h"
#include "time.h"
#include "transform.h"

TKinematic::TKinematic() :
	m_pTransform (nullptr),
	m_tSpeed(0,0),
	m_tAcceleration(0,0)
{

}

void TKinematic::MoveX(const TTime &tTime)
{
	const float fTimeDelta = tTime.GetTimeDeltaInSec();
	m_pTransform->m_tWorldPos.x += m_tSpeed.x * fTimeDelta + m_tAcceleration.x * (1 / 2.0f) * fTimeDelta * fTimeDelta;
	m_tSpeed.x += m_tAcceleration.x * fTimeDelta;
}

void TKinematic::MoveY(const TTime &tTime)
{
	const float fTimeDelta = tTime.GetTimeDeltaInSec();
	m_pTransform->m_tWorldPos.y += m_tSpeed.y * fTimeDelta + m_tAcceleration.y * (1 / 2.0f) * fTimeDelta * fTimeDelta;
	m_tSpeed.y += m_tAcceleration.y * fTimeDelta;
}

