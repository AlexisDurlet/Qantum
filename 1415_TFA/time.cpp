#include "time.h"
#include "flib.h"

void TTime::Initialize()
{
	m_iTimeNow = GfxTimeGetMilliseconds();
	m_iTimeBegin = m_iTimeNow;
	m_iTimePrevious = m_iTimeNow;
	m_iTimeFromBegin = 0;
}
void TTime::Update()
{
	m_iTimeNow = GfxTimeGetMilliseconds();
	m_iTimeFromBegin = m_iTimeNow - m_iTimeBegin;
	m_iTimeDelta = m_iTimeNow - m_iTimePrevious;
	m_iTimePrevious = m_iTimeNow;
}