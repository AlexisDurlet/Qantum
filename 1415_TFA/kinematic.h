#ifndef KINEMATIC_H_INCLUDED
#define	KINEMATIC_H_INCLUDED

class TTime;
class TTransform;

class TKinematic
{
	protected:

		TTransform * m_pTransform;

	public:

		TGfxVec2 m_tSpeed;
		TGfxVec2 m_tAcceleration;

		TKinematic();

		void SetTransform(TTransform * pTransform)
		{
			m_pTransform = pTransform;
		}
		
		void MoveX(const TTime &tTime);
		virtual void MoveY(const TTime &tTime);

};

#endif
