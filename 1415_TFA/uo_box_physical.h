#ifndef UO_BOX_PHYSICAL_H_INCLUDED
#define UO_BOX_PHYSICAL_H_INCLUDED

class TTime;

class TUoBoxPhysical : public TUoBoxCollider
{
	protected:

		TKinematicGravity m_tKinematicGravity;

		TTrigerLinked m_tInterfaceWithGround;

		bool m_bPhysical;
		bool m_bCollisionX;

		bool m_bGroundException;
		int m_iGroundExceptionFrameNumber;


		bool CheckForGround(const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber);
		void HandleGroundException();

		void UpdatePhysical(const TTime & tTime, const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber);
		void UpdateForced(const TTime & tTime, const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber);

	public:

		const TTrigerLinked * m_pExternalInterface;
		TGfxVec2 m_tExternalSpeed;

		void InitializeUoBoxPhysical(const TGfxVec2 & tWorldPos, const TGfxVec2 & tRadius, const TGfxVec2 & tSpeed, const TGfxVec2 & tAcceleration);
		virtual void Update(const TTime & tTime, const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber);

		void ResynchGroundInterface();
};


#endif
