#include "flib_vec2.h"
#include "transform.h"
#include "sprite.h"
#include "decoration.h"
#include "time.h"
#include "animation.h"
#include "decoration_animated.h"

void TDecorationAnimated::InitializeAnim(const ECutOutIncrement eCutOutIncrement, const int iTimePerFrame, const int iFrameNumber)
{
	m_tAnimation.Initialize(&m_tSprite, m_tSprite.GetInitialCutOut(), eCutOutIncrement, iTimePerFrame, iFrameNumber);
}

void TDecorationAnimated::StartAnim()
{
	m_tAnimation.Start();
}

void TDecorationAnimated::Update()
{
	TDecoration::Update();
	m_tAnimation.Update();
}
