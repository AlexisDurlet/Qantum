#include "flib_vec2.h"
#include "kinematic.h"
#include "time.h"
#include "kinematic_cycle.h"
#include "transform.h"
#include "lerp.h"

void TKinematicCycle::Initialize(TTransform * pTransform, const TGfxVec2 & tDestination, const int iStopTime, const float fSpeed)
{
	TKinematic::SetTransform(pTransform);

	m_tInitialPos = pTransform->GetWorldPos();
	m_tDestinationPos = tDestination;
	m_iStopTime = iStopTime;
	m_fSpeed = fSpeed;
	m_eCurrentState = ECycleState_Initial;
	m_eLastUpdateState = m_eCurrentState;
}

void TKinematicCycle::Update(const TTime & tTime)
{
	m_eLastUpdateState = m_eCurrentState;

	switch (m_eCurrentState)
	{
		case ECycleState_Initial:

			m_tTimer.Initialize();
			m_tSpeed = TGfxVec2(0, 0);
			m_tInitialSpeed = Lerpv(m_tInitialPos, m_tDestinationPos, m_fSpeed);
			m_fMovingTime = Lerpf(m_tInitialPos, m_tDestinationPos, m_fSpeed) * 1000;
			m_ePreviousState = m_eCurrentState;
			m_eCurrentState = ECycleState_Stopped;

			break;

		case ECycleState_Stopped:

			m_tTimer.Update();

			switch (m_ePreviousState)
			{
			case ECycleState_Initial:

				if (m_tTimer.GetTimeFromBegin() >= m_iStopTime)
				{
					m_tSpeed = m_tInitialSpeed;

					m_tTimer.Initialize();

					m_ePreviousState = m_eCurrentState;
					m_eCurrentState = ECycleState_MovingToDestination;
				}

				break;

			case ECycleState_MovingToInitial:

				if (m_tTimer.GetTimeFromBegin() >= m_iStopTime)
				{
					m_tSpeed = m_tInitialSpeed;

					m_tTimer.Initialize();

					m_ePreviousState = m_eCurrentState;
					m_eCurrentState = ECycleState_MovingToDestination;
				}

				break;

			case ECycleState_MovingToDestination:

				if (m_tTimer.GetTimeFromBegin() >= m_iStopTime)
				{
					m_tSpeed = -m_tInitialSpeed;

					m_tTimer.Initialize();

					m_ePreviousState = m_eCurrentState;
					m_eCurrentState = ECycleState_MovingToInitial;
				}

				break;

			default:

				break;
			}

			break;

		case ECycleState_MovingToInitial:

			m_tTimer.Update();

			if (m_tTimer.GetTimeFromBegin() < m_fMovingTime)
			{
				MoveX(tTime);
				MoveY(tTime);
			}
			else
			{
				m_pTransform->InitializeTransform(m_tInitialPos);

				m_tTimer.Initialize();

				m_tSpeed = TGfxVec2(0, 0);

				m_ePreviousState = m_eCurrentState;
				m_eCurrentState = ECycleState_Stopped;

			}

			break;

		case ECycleState_MovingToDestination:

			m_tTimer.Update();

			if (m_tTimer.GetTimeFromBegin() < m_fMovingTime)
			{
				MoveX(tTime);
				MoveY(tTime);
			}
			else
			{
				m_pTransform->InitializeTransform(m_tDestinationPos);

				m_tTimer.Initialize();

				m_tSpeed = TGfxVec2(0, 0);

				m_ePreviousState = m_eCurrentState;
				m_eCurrentState = ECycleState_Stopped;
			}

			break;
	}

}
