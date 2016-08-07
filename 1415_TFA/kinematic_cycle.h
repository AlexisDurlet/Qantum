#ifndef	KINEMATIC_CYCLE_H_INCLUDED
#define KINEMATIC_CYCLE_H_INCLUDED

enum ECycleState
{
	ECycleState_Initial = 0,
	ECycleState_Stopped,
	ECycleState_MovingToInitial,
	ECycleState_MovingToDestination,
};

class TKinematicCycle : public TKinematic
{
	private:

		TTime m_tTimer;

		TGfxVec2 m_tInitialPos;
		TGfxVec2 m_tDestinationPos;
		TGfxVec2 m_tInitialSpeed;

		float m_fSpeed;
		float m_fMovingTime;
		int m_iStopTime;

		ECycleState m_eCurrentState;
		ECycleState m_ePreviousState;
		ECycleState m_eLastUpdateState;

	public:

		ECycleState GetCurrentCycleState() const
		{
			return m_eCurrentState;
		}

		ECycleState GetPreviousCycleState() const
		{
			return m_ePreviousState;
		}

		ECycleState GetLastUpdateCycleState() const
		{
			return m_eLastUpdateState;
		}

		void Initialize(TTransform * pTransform, const TGfxVec2 & tDestination, const int iStopTime, const float fSpeed);
		void Update(const TTime & tTime);
};

#endif
