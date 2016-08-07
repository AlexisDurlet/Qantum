#include "fade.h"
#include "flib_vec2.h"
#include "transform.h"
#include "sprite.h"
#include "time.h"
#include "flib.h"

TFade::TFade():
m_fMinAlpha(0),
m_fMaxAlpha(255),
m_fAlphaValue(255)
{

}

void TFade::StartFadeIn(float fDelay)
{
	m_fAlphaSlope = (m_fMaxAlpha - m_fMinAlpha)/ fDelay;

	m_eFadeState = EFadeState_FadingIn;
}

void TFade::StartFadeOut(float fDelay)
{
	m_fAlphaSlope = (m_fMaxAlpha - m_fMinAlpha) / fDelay;

	m_eFadeState = EFadeState_FadingOut;
}

void TFade::Update(const TTime & tTime)
{
	switch (m_eFadeState)
	{
		case EFadeState_AllOpaque:

			m_fAlphaValue = m_fMaxAlpha;

			break;


		case EFadeState_AllTranslucent:

			m_fAlphaValue = m_fMinAlpha;

			break;


		case EFadeState_FadingIn:

			m_fAlphaValue = m_fAlphaValue - m_fAlphaSlope * tTime.GetTimeDelta();

			if (m_fAlphaValue <= m_fMinAlpha)
			{
				m_fAlphaValue = m_fMinAlpha;
				m_eFadeState = EFadeState_AllTranslucent;
			}

			break;


		case EFadeState_FadingOut:

			m_fAlphaValue = m_fAlphaValue + m_fAlphaSlope * tTime.GetTimeDelta();

			if (m_fAlphaValue >= m_fMaxAlpha)
			{
				m_fAlphaValue = m_fMaxAlpha;
				m_eFadeState = EFadeState_AllOpaque;
			}

			break;


		default:

			break;

	}

	m_pSprite->SetColor(GfxColor(0, 0, 0, m_fAlphaValue));
}
