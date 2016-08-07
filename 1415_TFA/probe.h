#ifndef PROBE_H_INCLUDED
#define PROBE_H_INCLUDED

class TUoBox;

class TProbe : private TTransform
{
	private:

		TGfxVec2 m_tLastValidPos;

		bool Collision(const TUoBox & tBoxCollided);

	public:

		TProbe();

		TGfxVec2 GetLastValidPos() const
		{
			return m_tLastValidPos;
		}

		void Initialize(const TGfxVec2 & tWorldPos)
		{
			InitializeTransform(tWorldPos);
		}

		const TUoBox * Probe(const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber);
		const TUoBox * RayProbe(const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber, const TGfxVec2 & tDestination, int fStepNumber);
};



#endif
