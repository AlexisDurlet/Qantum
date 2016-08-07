#include "clamp.h"
#include "flib_vec2.h"

float Clamp(const float fValue, const float fMin, const float fMax)
{
	return fValue < fMin ? fMin : (fValue > fMax ? fMax : fValue);
}

TGfxVec2 ClampVectorToBox(const TGfxVec2 & tVector, const TGfxVec2 & tBoxRadius)
{
	return TGfxVec2(Clamp(tVector.x, -tBoxRadius.x, tBoxRadius.x), Clamp(tVector.y, -tBoxRadius.y, tBoxRadius.y));
}