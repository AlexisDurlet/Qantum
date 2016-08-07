#ifndef _H_INCLUDED
#define _H_INCLUDED

struct TGfxVec2;

float Clamp(const float fValue, const float fMin, const float fMax);
	//Clamp a float between two others

TGfxVec2 ClampVectorToBox(const TGfxVec2 & tVector, const TGfxVec2 & tRadius);
	//Clamp a vector from a box center to the edges of the box 

#endif
