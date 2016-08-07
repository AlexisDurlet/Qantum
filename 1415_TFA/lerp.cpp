#include "flib_vec2.h"
#include "lerp.h"

float Lerpf(const TGfxVec2 & tInitial, const TGfxVec2 & tFinal, const float fIncrement)
{
	if (fIncrement > 0)
	{
		return (tFinal - tInitial).Length() / fIncrement;
	}

	return 0;
}

TGfxVec2 Lerpv(const TGfxVec2 & tInitial, const TGfxVec2 & tFinal, const float fIncrement)
{
	if (tFinal - tInitial != TGfxVec2(0, 0))
	{
		return (tFinal - tInitial).Normalize() * fIncrement;
	}
	else
	{
		return TGfxVec2(0, 0);
	}

}

