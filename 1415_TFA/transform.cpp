#include "flib_vec2.h"
#include "transform.h"

TTransform::TTransform():
	m_tWorldPos(TGfxVec2(0, 0))
{

}

void TTransform::InitializeTransform(const TGfxVec2 & tWorldPos)
{
	m_tWorldPos = tWorldPos;
}
