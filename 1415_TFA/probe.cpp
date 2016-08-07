#include "flib_vec2.h"
#include "transform.h"
#include "probe.h"
#include "uo_box.h"
#include "math.h"
#include "constants.h"

TProbe::TProbe()
{
	InitializeTransform(TGfxVec2(0, 0));
}

bool TProbe::Collision(const TUoBox & tBoxCollided)
{
	const TGfxVec2 tBoxCollidedPos = tBoxCollided.GetWorldPos();
	const TGfxVec2 tBoxCollidedRadius = tBoxCollided.GetRadius();

	const TGfxVec2 tProbeToBoxCollidedVector = tBoxCollidedPos - m_tWorldPos;
	const int iTolerance = 1;

	if (fabsf(tProbeToBoxCollidedVector.x) <= tBoxCollidedRadius.x + iTolerance && fabsf(tProbeToBoxCollidedVector.y) <= tBoxCollidedRadius.y + iTolerance)
	{
		return true;
	}

	return false;
}

const TUoBox * TProbe::Probe(const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber)
{
	for (int i = 0; i < iBoxesCollidedNumber; i++)
	{
		if (Collision(tBoxesCollided[i]))
		{
			return &tBoxesCollided[i];
		}
	}

	return nullptr;
}

const TUoBox * TProbe::RayProbe(const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber, const TGfxVec2 & tDestination, int iStepNumber)
{
	if (iStepNumber != 0)
	{
		const TGfxVec2 tProbeVector = tDestination - m_tWorldPos;
		const TGfxVec2 tProbeVectorNormalized = tProbeVector.Normalize();
		const float fStepLenght = tProbeVector.Length() / (iStepNumber * 1.0f);

		const TUoBox * tBoxProbed = nullptr;

		for (int i = 0; i < iStepNumber; i++)
		{
			m_tWorldPos += tProbeVectorNormalized * fStepLenght;
			tBoxProbed = Probe(tBoxesCollided, iBoxesCollidedNumber);
			if (tBoxProbed != nullptr)
			{
				m_tLastValidPos = m_tWorldPos - tProbeVectorNormalized * fStepLenght;
				return tBoxProbed;
			}
		}

		return tBoxProbed;
	}

	return nullptr;
}
