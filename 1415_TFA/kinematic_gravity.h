#ifndef KINEMATIC_GRAVITY_H_INCLUDED
#define	KINEMATIC_GRAVITY_H_INCLUDED

class TTransform;

class TKinematicGravity : public TKinematic
{
	public:

		bool m_bGravityOn;
		bool m_bOnGround;

		TKinematicGravity();

		bool GetGravityOn() const
		{
			return m_bGravityOn;
		}

		virtual void MoveY(const TTime &tTime);

};

#endif
