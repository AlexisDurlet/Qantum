#include "time.h"
#include "flib_vec2.h"
#include "animation.h"
#include "transform.h"
#include "sprite.h"
#include "flib.h"
#include "math.h"

TAnimation::TAnimation():
	m_pSprite(nullptr),
	m_bStarted(false)
{

}

void TAnimation::Initialize(TSprite * pSprite, const TGfxVec2 & tInitialCutout, const ECutOutIncrement eCutOutIncrement, const int iTimePerFrame, const int iFrameNumber)
{
	m_pSprite = pSprite;

	m_tInitialCutout = tInitialCutout;
	m_iTimePerFrame = iTimePerFrame;
	m_iFrameNumber = iFrameNumber;

	switch (eCutOutIncrement)
	{
		case ECutOutIncrement_X_Positive:

			m_tCutOutIncrement = TGfxVec2(m_pSprite->GetSize().x, 0);

			break;

		case ECutOutIncrement_X_Negative:

			m_tCutOutIncrement = TGfxVec2(-m_pSprite->GetSize().x, 0);

			break;

		case ECutOutIncrement_Y_Positive:

			m_tCutOutIncrement = TGfxVec2(0, m_pSprite->GetSize().y);

			break;

		case ECutOutIncrement_Y_Negative:

			m_tCutOutIncrement = TGfxVec2(0, -m_pSprite->GetSize().y);

			break;

	}
}

void TAnimation::InitializeOptional(const TGfxVec2 & tOptionalInitialCutOut)
{
	m_tOptionalInitialCutOut = tOptionalInitialCutOut;
}

int TAnimation::GetCycleDuration()
{
	return m_iFrameNumber * m_iTimePerFrame;
}

void TAnimation::Start()
{
	m_tTimer.Initialize();

	m_pSprite->SetCutOut(m_tInitialCutout);
	m_iCurrentFrame = 0;
	m_bStarted = true;
}

void TAnimation::Update()
{
	GfxDbgAssert(m_bStarted, "Animation hasn't been started");

	m_tTimer.Update();

	m_iCurrentFrame = (m_tTimer.GetTimeFromBegin() / m_iTimePerFrame) % m_iFrameNumber;
	const TGfxVec2 tCutout = m_tInitialCutout + (float(m_iCurrentFrame) * m_tCutOutIncrement);

	m_pSprite->SetCutOut(tCutout);
}

void TAnimation::SwitchInitialCutOut()
{
	TGfxVec2 tTemp;
	tTemp = m_tInitialCutout;
	m_tInitialCutout = m_tOptionalInitialCutOut;
	m_tOptionalInitialCutOut = tTemp;
}

void TAnimation::Reverse()
{
	m_tInitialCutout = m_tInitialCutout + float((m_iFrameNumber - 1)) * m_tCutOutIncrement;
	m_tOptionalInitialCutOut = m_tOptionalInitialCutOut + float((m_iFrameNumber - 1)) * m_tCutOutIncrement;
	m_tCutOutIncrement = -m_tCutOutIncrement;
}

TGfxVec2 TAnimation::GetLastFrameCutOut() const
{
	return m_tInitialCutout + float(m_iFrameNumber - 1) * m_tCutOutIncrement;
}
