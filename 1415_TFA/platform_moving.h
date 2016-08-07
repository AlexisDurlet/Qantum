#ifndef	PLATFORM_MOVING_H_INCLUDED
#define PLATFORM_MOVING_H_INCLUDED

class TUoBoxPhysical;

enum EPlatformState
{
	EPlatformState_Initial = 0,
	EPlatformState_Stopped,
	EPlatformState_MovingToInitial,
	EPlatformState_MovingToDestination,
};

class TPlatformMoving : public TPlatform
{
	private:

		TKinematicCycle m_tKinematic;
		TTrigerLinked m_tGround;

		void CheckPhysBoxOnGround(TUoBoxPhysical & tUoBoxPhysical);

	public:

		virtual void Initialize(const TGfxTexture * pTexture, const TGfxVec2 & tWorldPos, const TGfxVec2 & tRadius);
		void InitializeMovement(const TGfxVec2 & tDestination, const int iStopTime, const float fSpeed);
		virtual void Update(const TTime & tTime);
		virtual void Render();
};

#endif
